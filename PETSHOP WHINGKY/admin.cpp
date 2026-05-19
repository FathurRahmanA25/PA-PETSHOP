#include "toko.h"


//  Fitur: Tampilkan Tabel User
static void tampilkanTabelUser() {
    const int W_UID   = 4;
    const int W_UNAME = 20;
    const int W_UROLE = 8;
    const int W_UTGL  = 22;

    auto garis = [&]() {
        cout << BMAGENTA
             << "+" << string(W_UID+3,'-')
             << "+" << string(W_UNAME+2,'-')
             << "+" << string(W_UROLE+2,'-')
             << "+" << string(W_UTGL+2,'-')
             << "+" << RESET << "\n";
    };

    cout << "\n";
    garis();
    cout << BMAGENTA << "|" << RESET
         << BG_BLUE << BWHITE << setw(W_UID+3)    << right << " ID "          << RESET << BMAGENTA << "|" << RESET
         << BG_BLUE << BWHITE << " " << left << setw(W_UNAME+1) << "Username"       << RESET << BMAGENTA << "|" << RESET
         << BG_BLUE << BWHITE << " " << left << setw(W_UROLE+1) << "Role"           << RESET << BMAGENTA << "|" << RESET
         << BG_BLUE << BWHITE << " " << left << setw(W_UTGL+1)  << "Tanggal Daftar" << RESET << BMAGENTA << "|" << RESET << "\n";
    garis();

    for (int i = 0; i < jumlahPengguna; i++) {
        const Pengguna& u = daftarPengguna[i];
        cout << BMAGENTA << "|" << RESET
             << BWHITE << setw(W_UID+2) << right << u.id << " " << RESET << BMAGENTA << "|" << RESET;
        if (u.role == "admin")
            cout << BYELLOW << " " << left << setw(W_UNAME+1) << u.username << RESET << BMAGENTA << "|" << RESET;
        else
            cout << BCYAN   << " " << left << setw(W_UNAME+1) << u.username << RESET << BMAGENTA << "|" << RESET;
        cout << " " << left << setw(W_UROLE+1) << u.role          << BMAGENTA << "|" << RESET
             << " " << left << setw(W_UTGL+1)  << u.tanggalDaftar << BMAGENTA << "|" << RESET << "\n";
        garis();
    }
}


//  FITUR: Sub-Menu Kelola User (Lihat & Hapus)
static void subMenuKelolaUser() {
    clearLayar();
    cout << "\n"
         << BMAGENTA << "  +================================================+\n"
         << "  |" << RESET;
    cout << BG_BLUE << BWHITE << pusatTeks("KELOLA PENGGUNA", 48) << RESET;
    cout << BMAGENTA << "|\n"
         << "  +------------------------------------------------+\n"
         << "  |" << RESET << "  " << BCYAN   << "[ 1 ]" << RESET << "  Lihat Semua Pengguna                " << BMAGENTA << "   |\n"
         << "  |" << RESET << "  " << BRED    << "[ 2 ]" << RESET << "  Hapus Pengguna                      " << BMAGENTA << "   |\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 0 ]" << RESET << "  Kembali                             " << BMAGENTA << "   |\n"
         << "  +================================================+\n" << RESET;
    cout << BYELLOW << "  Pilih [0-2]" << RESET;
    int p = inputAngkaRentang(0, 2);

    if (p == 0) return;

    if (p == 1) {
        animasiLoading("Memuat data pengguna", 2);
        tampilkanTabelUser();
        cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // p == 2: Hapus Pengguna
    animasiLoading("Memuat data pengguna", 2);
    tampilkanTabelUser();

    cout << BYELLOW << "\n  Masukkan ID Pengguna yang akan dihapus" << RESET;
    int id = inputAngkaAman();
    if (id <= 0) { cetakPesan("err", "ID tidak valid."); return; }

    int idx = -1;
    for (int i = 0; i < jumlahPengguna; i++) {
        if (daftarPengguna[i].id == id) { idx = i; break; }
    }
    if (idx == -1) { cetakPesan("warn", "Pengguna dengan ID tersebut tidak ditemukan."); return; }

    // Proteksi: tidak boleh hapus satu-satunya admin
    if (daftarPengguna[idx].role == "admin") {
        int adminCount = 0;
        for (int i = 0; i < jumlahPengguna; i++)
            if (daftarPengguna[i].role == "admin") adminCount++;
        if (adminCount <= 1) {
            cetakPesan("err", "Tidak dapat menghapus satu-satunya akun admin.");
            return;
        }
    }

    cout << BRED << "\n  Yakin hapus pengguna '" << BWHITE << daftarPengguna[idx].username
         << BRED << "' (Role: " << daftarPengguna[idx].role << ")?\n" << RESET
         << "  [ 1 ] Ya, Hapus   [ 0 ] Tidak, Batalkan\n";
    cout << BYELLOW << "  Pilih [0/1]" << RESET;
    int konfirm = inputAngkaRentang(0, 1);

    if (konfirm == 1) {
        string namaTerhapus = daftarPengguna[idx].username;
        animasiLoading("Menghapus pengguna", 2);
        for (int i = idx; i < jumlahPengguna - 1; i++)
            daftarPengguna[i] = daftarPengguna[i + 1];
        jumlahPengguna--;
        cetakPesan("ok", "Pengguna '" + namaTerhapus + "' berhasil dihapus.");
    } else {
        cetakPesan("info", "Penghapusan dibatalkan.");
    }
}


//  FITUR: Sub-Menu Pencarian Produk
static void subMenuCariProduk() {
    clearLayar();
    cout << "\n"
         << BMAGENTA << "  +============================================+\n"
         << "  |" << RESET;
    cout << BG_BLUE << BWHITE << pusatTeks("CARI PRODUK", 44) << RESET;
    cout << BMAGENTA << "|\n"
         << "  +--------------------------------------------+\n"
         << "  |" << RESET << "  " << BCYAN   << "[ 1 ]" << RESET << "  Cari berdasarkan Nama / Keyword  " << BMAGENTA << "  |\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 2 ]" << RESET << "  Cari berdasarkan ID Produk       " << BMAGENTA << "  |\n"
         << "  +============================================+\n" << RESET;
    cout << BYELLOW << "  Pilih Metode Pencarian [1-2]" << RESET;
    int p = inputAngkaRentang(1, 2);

    if (p == 1) {
        string kw = inputTeksAman("Masukkan keyword: ");
        animasiLoading("Mencari produk", 2);
        auto hasil = cariLinearKeyword(kw);
        if (hasil.empty()) {
            cetakPesan("warn", "Tidak ada produk yang cocok dengan keyword tersebut.");
        } else {
            Produk temp[MAX_PRODUK];
            for (size_t i = 0; i < hasil.size(); i++) temp[i] = daftarProduk[hasil[i]];
            tampilkanTabelProduk(temp, (int)hasil.size());
        }
    } else {
        cout << BYELLOW << "  Masukkan ID Produk" << RESET;
        int id = inputAngkaAman();
        if (id <= 0) { cetakPesan("err", "ID harus lebih dari 0."); return; }
        animasiLoading("Mencari produk", 2);
        int idx = cariBinaryByID(id);
        if (idx == -1) {
            cetakPesan("warn", "Produk dengan ID tersebut tidak ditemukan.");
        } else {
            tampilkanTabelProduk(&daftarProduk[idx], 1);
        }
    }
    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//  FITUR: Sub-Menu Filter & Urutkan
static void subMenuFilterDanSort() {
    clearLayar();
    cout << "\n"
         << BBLUE << "  +================================================+\n"
         << "  |" << RESET;
    cout << BG_BLUE << BWHITE << pusatTeks("FILTER & URUTKAN PRODUK", 48) << RESET;
    cout << BBLUE << "|\n"
         << "  +------------------------------------------------+\n"
         << "  |" << RESET << "  " << BCYAN << "Pilih Kategori Hewan:" << string(25,' ') << BBLUE << "|\n"
         << "  |" << RESET << "  [ 1 ]  Kucing" << string(33,' ') << BBLUE << "|\n"
         << "  |" << RESET << "  [ 2 ]  Anjing" << string(33,' ') << BBLUE << "|\n"
         << "  |" << RESET << "  [ 3 ]  Reptil" << string(33,' ') << BBLUE << "|\n"
         << "  |" << RESET << "  [ 4 ]  Hewan Ternak" << string(27,' ') << BBLUE << "|\n"
         << "  +================================================+\n" << RESET;
    cout << BYELLOW << "  Pilih Kategori [1-4]" << RESET;
    int kat = inputAngkaRentang(1, 4);

    cout << "\n"
         << BBLUE << "  +======================================+\n"
         << "  |" << RESET << "  " << BCYAN << "Urutan Harga:" << string(23,' ') << BBLUE << "|\n"
         << "  |" << RESET << "  [ 1 ]   Termurah ke Termahal        " << BBLUE << "|\n"
         << "  |" << RESET << "  [ 2 ]   Termahal ke Termurah        " << BBLUE << "|\n"
         << "  +======================================+\n" << RESET;
    cout << BYELLOW << "  Pilih Urutan [1-2]" << RESET;
    int urut = inputAngkaRentang(1, 2);

    animasiLoading("Memfilter dan mengurutkan", 2);
    filterKategoriDanSort((KategoriHewan)kat, (UrutanHarga)urut);
    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}



//  FITUR: Sub-Menu Sorting
static void subMenuSorting() {
    clearLayar();
    cout << "\n"
         << BCYAN << "  +====================================================+\n"
         << "  |" << RESET;
    cout << BG_CYAN << BWHITE << pusatTeks("URUTKAN SEMUA PRODUK", 52) << RESET;
    cout << BCYAN << "|\n"
         << "  +----------------------------------------------------+\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 1 ]" << RESET << "    Harga Termurah ke Termahal  (Bubble)     " << BCYAN << "|\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 2 ]" << RESET << "    Harga Termahal ke Termurah  (Bubble)     " << BCYAN << "|\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 3 ]" << RESET << "    Nama Produk A ke Z          (Selection)  " << BCYAN << "|\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 4 ]" << RESET << "    Stok Paling Sedikit         (Insertion)  " << BCYAN << "|\n"
         << "  +====================================================+\n" << RESET;
    cout << BYELLOW << "  Pilih Metode [1-4]" << RESET;
    int metode = inputAngkaRentang(1, 4);

    Produk temp[MAX_PRODUK];
    for (int i = 0; i < jumlahProduk; i++) temp[i] = daftarProduk[i];

    string label;
    animasiLoading("Mengurutkan produk", 3);
    switch (metode) {
        case 1: bubbleSortHarga(temp, jumlahProduk, ASC);  label = "Harga: Termurah ke Termahal"; break;
        case 2: bubbleSortHarga(temp, jumlahProduk, DESC); label = "Harga: Termahal ke Termurah"; break;
        case 3: selectionSortNama(temp, jumlahProduk);     label = "Nama Produk: A ke Z";         break;
        case 4: insertionSortStok(temp, jumlahProduk);     label = "Stok: Paling Sedikit";        break;
    }

    cout << "\n" << BCYAN << "  +-- Hasil Sorting: " << BWHITE << label << RESET << "\n";
    tampilkanTabelProduk(temp, jumlahProduk);
    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


//  FITUR: Sub-Menu Lihat Produk & Riwayat
static void subMenuLihat() {
    clearLayar();
    cout << "\n"
         << BBLUE << "  +============================================+\n"
         << "  |" << RESET;
    cout << BG_BLUE << BWHITE << pusatTeks("MENU LIHAT DATA", 44) << RESET;
    cout << BBLUE << "|\n"
         << "  +--------------------------------------------+\n"
         << "  |" << RESET << "  " << BCYAN   << "[ 1 ]" << RESET << "  Lihat Semua Produk              " << BBLUE << "   |\n"
         << "  |" << RESET << "  " << BYELLOW << "[ 2 ]" << RESET << "  Riwayat Pembelian Pelanggan     " << BBLUE << "   |\n"
         << "  +============================================+\n" << RESET;
    cout << BYELLOW << "  Pilih [1-2]" << RESET;
    int p = inputAngkaRentang(1, 2);
    if (p == 1) {
        animasiLoading("Memuat data produk", 2);
        tampilkanTabelProduk(daftarProduk, jumlahProduk);
    } else {
        animasiLoading("Memuat riwayat", 2);
        tampilkanRiwayat("admin");
    }
    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


//  FITUR: Panel Admin
void menuAdmin() {
    int pilihan;
    do {
        clearLayar();
        cout << "\n\n"
             << BYELLOW
             << "  +======================================================================+\n"
             << "  |" << RESET;
        cout << BG_YELLOW << "\033[30m" << pusatTeks("[ PANEL ADMIN ]  TOKO WHINGKY", 70) << RESET;
        cout << BYELLOW << "|\n"
             << "  +----------------------------------------------------------------------+\n"
             << "  |" << RESET << "  " << BCYAN    << "[ 1 ]" << RESET << "  Lihat Produk & Riwayat" << string(39,' ') << BYELLOW << "|\n"
             << "  |" << RESET << "  " << BMAGENTA << "[ 2 ]" << RESET << "  Cari Produk  (Nama / ID)" << string(37,' ') << BYELLOW << "|\n"
             << "  |" << RESET << "  " << BBLUE    << "[ 3 ]" << RESET << "  Filter Kategori & Urutkan Harga" << string(30,' ') << BYELLOW << "|\n"
             << "  |" << RESET << "  " << BCYAN    << "[ 4 ]" << RESET << "  Sorting Produk" << string(47,' ') << BYELLOW << "|\n"
             << "  +----------------------------------------------------------------------+\n"
             << "  |" << RESET << "  " << BGREEN   << "[ 5 ]" << RESET << "  Tambah Produk Baru" << string(43,' ') << BYELLOW << "|\n"
             << "  |" << RESET << "  " << BBLUE    << "[ 6 ]" << RESET << "  Perbarui Data Produk" << string(41,' ') << BYELLOW << "|\n"
             << "  |" << RESET << "  " << BRED     << "[ 7 ]" << RESET << "  Hapus Produk" << string(49,' ') << BYELLOW << "|\n"
             << "  +----------------------------------------------------------------------+\n"
             << "  |" << RESET << "  " << BMAGENTA << "[ 8 ]" << RESET << "  Kelola Pengguna" << string(46,' ') << BYELLOW << "|\n"
             << "  +----------------------------------------------------------------------+\n"
             << "  |" << RESET << "  " << BRED     << "[ 0 ]" << RESET << "  Keluar dari Panel Admin" << string(38,' ') << BYELLOW << "|\n"
             << "  +======================================================================+\n" << RESET;

        cout << "\n" << BYELLOW << "  Pilih Menu [0-8]" << RESET;
        pilihan = inputAngkaRentang(0, 8);

        try {
            switch (pilihan) {
                case 1: subMenuLihat();        break;
                case 2: subMenuCariProduk();   break;
                case 3: subMenuFilterDanSort(); break;
                case 4: subMenuSorting();      break;

                case 5: {
                    clearLayar();
                    if (jumlahProduk >= MAX_PRODUK) lemparError("Kapasitas produk sudah penuh");
                    cout << "\n";
                    cetakHeader("TAMBAH PRODUK BARU", 52, BGREEN);
                    Produk p;
                    p.id   = jumlahProduk + 1;
                    p.nama = inputTeksAman("Nama Produk         : ");

                    cout << BCYAN << "\n  Kategori:\n" << RESET
                         << "  [ 1 ] Kucing  [ 2 ] Anjing  [ 3 ] Reptil  [ 4 ] Hewan Ternak\n";
                    cout << BYELLOW << "  Pilih Kategori [1-4]" << RESET;
                    p.kategori = (KategoriHewan)inputAngkaRentang(1, 4);

                    cout << BCYAN << "\n  Sub Kategori:\n" << RESET
                         << "  [ 1 ] Obat  [ 2 ] Makanan  [ 3 ] Aksesoris  [ 4 ] Perlengkapan\n";
                    cout << BYELLOW << "  Pilih Sub Kategori [1-4]" << RESET;
                    p.subKategori = (SubKategori)inputAngkaRentang(1, 4);

                    cout << BCYAN << "  Harga (Rp)" << RESET;
                    p.harga = inputDoubleAman();
                    validasiHarga(p.harga);

                    cout << BCYAN << "  Jumlah Stok" << RESET;
                    p.stok = inputAngkaAman();
                    if (p.stok < 0) lemparError("Stok tidak boleh negatif");

                    animasiLoading("Menyimpan produk", 2);
                    daftarProduk[jumlahProduk++] = p;
                    cetakPesan("ok", "Produk '" + p.nama + "' berhasil ditambahkan (ID #" + to_string(p.id) + ")");
                    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali ke menu..." << RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                case 6: {
                    clearLayar();
                    cout << "\n";
                    cetakHeader("PERBARUI DATA PRODUK", 52, BBLUE);
                    tampilkanTabelProduk(daftarProduk, jumlahProduk);
                    cout << BYELLOW << "\n  Masukkan ID Produk yang akan diperbarui" << RESET;
                    int id = inputAngkaAman();
                    if (id <= 0) { cetakPesan("err", "ID tidak valid."); break; }
                    animasiLoading("Mencari produk", 2);
                    int idx = cariBinaryByID(id);
                    if (idx == -1) { cetakPesan("warn", "Produk tidak ditemukan."); break; }

                    tampilkanTabelProduk(&daftarProduk[idx], 1);
                    cout << BCYAN << "\n  Field yang akan diperbarui:\n" << RESET
                         << "  [ 1 ] Nama   [ 2 ] Harga   [ 3 ] Stok\n";
                    cout << BYELLOW << "  Pilih Field [1-3]" << RESET;
                    int field = inputAngkaRentang(1, 3);

                    if (field == 1) {
                        daftarProduk[idx].nama = inputTeksAman("Nama baru: ");
                    } else if (field == 2) {
                        cout << BCYAN << "  Harga baru (Rp)" << RESET;
                        double h = inputDoubleAman();
                        validasiHarga(h);
                        daftarProduk[idx].harga = h;
                    } else {
                        cout << BCYAN << "  Stok baru" << RESET;
                        int s = inputAngkaAman();
                        if (s < 0) lemparError("Stok tidak boleh negatif");
                        daftarProduk[idx].stok = s;
                    }
                    cetakPesan("ok", "Data produk berhasil diperbarui.");
                    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali ke menu..." << RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                case 7: {
                    clearLayar();
                    cout << "\n";
                    cetakHeader("HAPUS PRODUK", 52, BRED);
                    // Tampilkan SEMUA produk supaya admin bisa pilih ID
                    animasiLoading("Memuat daftar produk", 2);
                    tampilkanTabelProduk(daftarProduk, jumlahProduk);
                    cout << BYELLOW << "\n  Masukkan ID Produk yang akan dihapus" << RESET;
                    int id = inputAngkaAman();
                    if (id <= 0) { cetakPesan("err", "ID tidak valid."); break; }
                    animasiLoading("Mencari produk", 2);
                    int idx = cariBinaryByID(id);
                    if (idx == -1) { cetakPesan("warn", "Produk tidak ditemukan."); break; }

                    tampilkanTabelProduk(&daftarProduk[idx], 1);
                    cout << BRED << "\n  Yakin hapus produk '" << BWHITE << daftarProduk[idx].nama
                         << BRED << "'?\n" << RESET
                         << "  [ 1 ] Ya, Hapus   [ 0 ] Tidak, Batalkan\n";
                    cout << BYELLOW << "  Pilih [0/1]" << RESET;
                    int konfirm = inputAngkaRentang(0, 1);
                    if (konfirm == 1) {
                        string namaTerhapus = daftarProduk[idx].nama;
                        animasiLoading("Menghapus produk", 2);
                        for (int i = idx; i < jumlahProduk - 1; i++)
                            daftarProduk[i] = daftarProduk[i + 1];
                        jumlahProduk--;
                        cetakPesan("ok", "Produk '" + namaTerhapus + "' berhasil dihapus.");
                    } else {
                        cetakPesan("info", "Penghapusan dibatalkan.");
                    }
                    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali ke menu..." << RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }

                case 8:
                    subMenuKelolaUser();
                    break;

                case 0:
                    animasiLoading("Keluar dari panel admin", 2);
                    clearLayar();
                    cetakPesan("info", "Keluar dari Panel Admin.");
                    break;
            }
        } catch (const exception& e) {
            cetakPesan("err", e.what());
        }
    } while (pilihan != 0);
}