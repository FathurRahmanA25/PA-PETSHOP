//   SISTEM MANAJEMEN TOKO PERLENGKAPAN HEWAN "WHINGKY"
//   File     : user.cpp
//   Deskripsi: Menu Pelanggan -- lihat produk, keranjang,
//              proses pembelian, struk, dan riwayat

#include "toko.h"
#include <fstream>  //tambahan buat nulis file HTML (ofstream)


//  FITUR: Generate file HTML berisi QR Code lalu buka di browser
//
//  Cara kerjanya:
//    1. Bikin file "qr_pembayaran.html" di folder yang sama sama .exe
//    2. Di dalem HTML itu kita pake library qrcode.js (dari CDN, butuh internet)
//       -- library itu yang otomatis gambar kotak-kotak QR-nya
//    3. Setelah file jadi, kita panggil perintah OS buat buka file itu di browser
//
//  Parameter:
//    - username  : nama pembeli (ditampilin di halaman)
//    - total     : total yang harus dibayar
//    - timestamp : waktu transaksi (disamain sama yang di struk)
static void tampilkanQRCode(const string& username, double total, const string& timestamp) {

    // isi QR itu cuma string teks biasa -- nanti library JS yang encode jadi gambar kotaknya
    // formatnya bebas, kita bikin sendiri biar keliatan kayak data transaksi beneran
    string qrData = "WHINGKY-QRIS|PEMBELI:" + username
                  + "|TOTAL:Rp" + to_string((long long)total)
                  + "|WAKTU:" + timestamp
                  + "|MERCHANT:TokoWhingky";

    string namaFile = "qr_pembayaran.html";   // nama file HTML yang bakal dibuat

    // ofstream = buat nulis ke file; kalau file belum ada, otomatis dibikin
    ofstream fp(namaFile);
    if (!fp.is_open()) {
        cetakPesan("err", "Gagal membuat file QR. Cek permission folder.");
        return;
    }

    // ── Tulis konten HTML ──
    // Ini HTML biasa yang di-generate dari C++; hasilnya dibuka di browser
    fp << "<!DOCTYPE html>\n";
    fp << "<html lang='id'>\n";
    fp << "<head>\n";
    fp << "  <meta charset='UTF-8'>\n";
    fp << "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    fp << "  <title>Bayar via QRIS - Toko Whingky</title>\n";

    // qrcode.js dari CDN -- library ini yang otomatis gambar QR-nya, butuh internet
    fp << "  <script src='https://cdnjs.cloudflare.com/ajax/libs/qrcodejs/1.0.0/qrcode.min.js'></script>\n";

    fp << "  <link rel='preconnect' href='https://fonts.googleapis.com'>\n";
    fp << "  <link href='https://fonts.googleapis.com/css2?family=Space+Mono:wght@400;700&family=Syne:wght@700;800&display=swap' rel='stylesheet'>\n";

    // ── CSS ──
    fp << "  <style>\n";

    fp << "    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }\n";
    fp << "    body {\n";
    fp << "      font-family: 'Space Mono', monospace;\n";
    fp << "      background: #0d0d14;\n";
    fp << "      min-height: 100vh;\n";
    fp << "      display: flex;\n";
    fp << "      align-items: center;\n";
    fp << "      justify-content: center;\n";
    fp << "      padding: 24px;\n";
    fp << "      position: relative;\n";
    fp << "      overflow: hidden;\n";
    fp << "    }\n";

    // efek cincin dekoratif di background (pure CSS, ga ganggu konten)
    fp << "    body::before {\n";
    fp << "      content: '';\n";
    fp << "      position: fixed;\n";
    fp << "      top: -200px; left: -200px;\n";
    fp << "      width: 600px; height: 600px;\n";
    fp << "      border-radius: 50%;\n";
    fp << "      background: radial-gradient(circle, rgba(250,204,21,0.06) 0%, transparent 70%);\n";
    fp << "      pointer-events: none;\n";
    fp << "    }\n";
    fp << "    body::after {\n";
    fp << "      content: '';\n";
    fp << "      position: fixed;\n";
    fp << "      bottom: -150px; right: -150px;\n";
    fp << "      width: 500px; height: 500px;\n";
    fp << "      border-radius: 50%;\n";
    fp << "      background: radial-gradient(circle, rgba(99,102,241,0.08) 0%, transparent 70%);\n";
    fp << "      pointer-events: none;\n";
    fp << "    }\n";

    fp << "    .card {\n";
    fp << "      background: #13131f;\n";
    fp << "      border: 1px solid #2a2a3d;\n";
    fp << "      border-radius: 20px;\n";
    fp << "      padding: 36px 40px;\n";
    fp << "      max-width: 420px;\n";
    fp << "      width: 100%;\n";
    fp << "      text-align: center;\n";
    fp << "      position: relative;\n";
    fp << "      z-index: 1;\n";
    fp << "      box-shadow: 0 0 60px rgba(250,204,21,0.04), 0 24px 64px rgba(0,0,0,0.5);\n";
    fp << "      animation: fadeUp 0.5s ease both;\n";
    fp << "    }\n";

    // animasi masuk: card slide dari bawah + fade in
    fp << "    @keyframes fadeUp {\n";
    fp << "      from { opacity: 0; transform: translateY(24px); }\n";
    fp << "      to   { opacity: 1; transform: translateY(0); }\n";
    fp << "    }\n";

    // garis aksen kuning tipis di tepi atas card
    fp << "    .card::before {\n";
    fp << "      content: '';\n";
    fp << "      position: absolute;\n";
    fp << "      top: 0; left: 10%; right: 10%;\n";
    fp << "      height: 2px;\n";
    fp << "      background: linear-gradient(90deg, transparent, #facc15, transparent);\n";
    fp << "      border-radius: 2px;\n";
    fp << "    }\n";

    fp << "    .brand {\n";
    fp << "      font-family: 'Syne', sans-serif;\n";
    fp << "      font-weight: 800;\n";
    fp << "      font-size: 22px;\n";
    fp << "      letter-spacing: 0.12em;\n";
    fp << "      color: #facc15;\n";
    fp << "      margin-bottom: 4px;\n";
    fp << "    }\n";
    fp << "    .tagline {\n";
    fp << "      font-size: 10px;\n";
    fp << "      color: #4a4a6a;\n";
    fp << "      letter-spacing: 0.2em;\n";
    fp << "      text-transform: uppercase;\n";
    fp << "      margin-bottom: 24px;\n";
    fp << "    }\n";
    fp << "    .divider {\n";
    fp << "      border: none;\n";
    fp << "      border-top: 1px solid #1e1e30;\n";
    fp << "      margin: 20px 0;\n";
    fp << "    }\n";
    fp << "    .label {\n";
    fp << "      font-size: 10px;\n";
    fp << "      color: #4a4a6a;\n";
    fp << "      letter-spacing: 0.15em;\n";
    fp << "      text-transform: uppercase;\n";
    fp << "      margin-bottom: 6px;\n";
    fp << "    }\n";
    fp << "    .buyer {\n";
    fp << "      font-size: 16px;\n";
    fp << "      color: #c0c0e0;\n";
    fp << "      font-weight: 700;\n";
    fp << "      margin-bottom: 20px;\n";
    fp << "    }\n";
    fp << "    .total-amount {\n";
    fp << "      font-family: 'Syne', sans-serif;\n";
    fp << "      font-size: 36px;\n";
    fp << "      font-weight: 800;\n";
    fp << "      color: #ffffff;\n";
    fp << "      letter-spacing: -0.02em;\n";
    fp << "      margin-bottom: 24px;\n";
    fp << "      line-height: 1;\n";
    fp << "    }\n";
    fp << "    .total-amount span { color: #facc15; }\n";   // "Rp" warna kuning
    fp << "    .qr-wrapper {\n";
    fp << "      background: #ffffff;\n";                    // harus putih, QR default hitam-putih
    fp << "      border-radius: 16px;\n";
    fp << "      padding: 20px;\n";
    fp << "      display: inline-block;\n";
    fp << "      margin-bottom: 20px;\n";
    fp << "      box-shadow: 0 0 0 1px rgba(250,204,21,0.15), 0 8px 32px rgba(0,0,0,0.4);\n";
    fp << "    }\n";
    fp << "    #qrcode { display: block; }\n";
    fp << "    #qrcode img, #qrcode canvas { display: block; }\n";
    fp << "    .qris-badge {\n";
    fp << "      display: inline-flex;\n";
    fp << "      align-items: center;\n";
    fp << "      gap: 6px;\n";
    fp << "      background: #facc15;\n";
    fp << "      color: #0d0d14;\n";
    fp << "      font-size: 11px;\n";
    fp << "      font-weight: 700;\n";
    fp << "      letter-spacing: 0.1em;\n";
    fp << "      padding: 6px 16px;\n";
    fp << "      border-radius: 999px;\n";
    fp << "      margin-bottom: 16px;\n";
    fp << "    }\n";
    fp << "    .instruction {\n";
    fp << "      font-size: 12px;\n";
    fp << "      color: #5a5a7a;\n";
    fp << "      line-height: 1.6;\n";
    fp << "      margin-bottom: 20px;\n";
    fp << "    }\n";
    fp << "    .timestamp {\n";
    fp << "      font-size: 10px;\n";
    fp << "      color: #2e2e46;\n";
    fp << "      letter-spacing: 0.05em;\n";
    fp << "    }\n";
    fp << "  </style>\n";
    fp << "</head>\n";
    fp << "<body>\n";

    // ── Konten HTML ──
    fp << "<body>\n";
    fp << "  <div class='card'>\n";
    fp << "    <div class='brand'>&#128062; WHINGKY</div>\n";
    fp << "    <div class='tagline'>Toko Perlengkapan Hewan</div>\n";
    fp << "    <hr class='divider'>\n";
    fp << "    <div class='label'>Pembeli</div>\n";
    fp << "    <div class='buyer'>" << username << "</div>\n";
    fp << "    <div class='label'>Total Pembayaran</div>\n";
    fp << "    <div class='total-amount'><span>Rp</span> " << to_string((long long)total) << "</div>\n";
    fp << "    <div class='qris-badge'>&#9646;&#9646; QRIS</div>\n";
    fp << "    <div class='qr-wrapper'>\n";
    fp << "      <div id='qrcode'></div>\n";   // div kosong ini yang bakal diisi gambar QR oleh JS
    fp << "    </div>\n";
    fp << "    <p class='instruction'>\n";
    fp << "      Buka aplikasi e-wallet atau m-banking kamu,<br>\n";
    fp << "      lalu scan QR di atas untuk menyelesaikan pembayaran.\n";
    fp << "    </p>\n";
    fp << "    <hr class='divider'>\n";
    fp << "    <div class='timestamp'>" << timestamp << "</div>\n";
    fp << "  </div>\n";

    // ── JavaScript: panggil QRCode.js untuk generate gambar QR ──
    // new QRCode(element, options) -- element = div yang mau diisi, options = pengaturan QR
    fp << "  <script>\n";
    fp << "    new QRCode(document.getElementById('qrcode'), {\n";
    fp << "      text: '" << qrData << "',\n";   // data yang di-encode ke gambar QR
    fp << "      width:  220,\n";                 // lebar gambar QR (pixel)
    fp << "      height: 220,\n";                 // tinggi gambar QR (pixel)
    fp << "      colorDark:  '#000000',\n";       // warna kotak QR
    fp << "      colorLight: '#ffffff',\n";       // warna background QR (harus putih)
    fp << "      correctLevel: QRCode.CorrectLevel.H\n";  // H = error correction tertinggi
    fp << "    });\n";
    fp << "  </script>\n";
    fp << "</body>\n";
    fp << "</html>\n";

    fp.close();   // tutup file setelah selesai nulis

    // ── Buka file HTML di browser default sesuai OS ──
    // tiap OS punya perintah shell yang beda buat "buka file ini pake browser"
    string perintahBuka;
#ifdef _WIN32
    perintahBuka = "start " + namaFile;       // Windows
#elif __APPLE__
    perintahBuka = "open " + namaFile;        // macOS
#else
    perintahBuka = "xdg-open " + namaFile;   // Linux
#endif
    system(perintahBuka.c_str());   // system() = jalanin perintah shell dari C++
}


//  FITUR: Sub-Menu Cari & Filter Produk (Pelanggan)
static void subMenuCariFilterUser() {
    cout << "\n"
         << BMAGENTA << "  +==================================================+\n"
         << "  |" << RESET;
    cout << BG_BLUE << BWHITE << pusatTeks("CARI & FILTER PRODUK", 50) << RESET;
    cout << BMAGENTA << "|\n"
         << "  +--------------------------------------------------+\n"
         << "  |" << RESET << "  " << BCYAN    << "[ 1 ]" << RESET << "   Cari berdasarkan Nama / Keyword        " << BMAGENTA << " |\n"
         << "  |" << RESET << "  " << BYELLOW  << "[ 2 ]" << RESET << "   Cari berdasarkan ID Produk             " << BMAGENTA << " |\n"
         << "  |" << RESET << "  " << BBLUE    << "[ 3 ]" << RESET << "   Filter Kategori & Urutkan Harga        " << BMAGENTA << " |\n"
         << "  +==================================================+\n" << RESET;
    cout << BYELLOW << "  Pilih [1-3]" << RESET;
    int p = inputAngkaRentang(1, 3);

    if (p == 1) {
        string kw = inputTeksAman("Masukkan keyword: ");
        animasiLoading("Mencari produk", 2);
        auto hasil = cariLinearKeyword(kw);
        if (hasil.empty()) {
            cetakPesan("warn", "Tidak ada produk yang cocok.");
        } else {
            Produk temp[MAX_PRODUK];
            for (size_t i = 0; i < hasil.size(); i++) temp[i] = daftarProduk[hasil[i]];
            tampilkanTabelProduk(temp, (int)hasil.size());
        }
    } else if (p == 2) {
        cout << BYELLOW << "  Masukkan ID Produk" << RESET;
        int id = inputAngkaAman();
        if (id <= 0) { cetakPesan("err", "ID tidak valid."); return; }
        animasiLoading("Mencari produk", 2);
        int idx = cariBinaryByID(id);
        if (idx == -1) {
            cetakPesan("warn", "Produk tidak ditemukan.");
        } else {
            tampilkanTabelProduk(&daftarProduk[idx], 1);
        }
    } else {
        cout << "\n" << BBLUE
             << "  +==========================================+\n"
             << "  |" << RESET;
        cout << BG_BLUE << BWHITE << pusatTeks("Pilih Kategori Hewan", 42) << RESET;
        cout << BBLUE << "|\n"
             << "  +------------------------------------------+\n"
             << "  |" << RESET << "  [ 1 ]  Kucing" << string(27,' ') << BBLUE << "|\n"
             << "  |" << RESET << "  [ 2 ]  Anjing" << string(27,' ') << BBLUE << "|\n"
             << "  |" << RESET << "  [ 3 ]  Reptil" << string(27,' ') << BBLUE << "|\n"
             << "  |" << RESET << "  [ 4 ]  Hewan Ternak" << string(21,' ') << BBLUE << "|\n"
             << "  +==========================================+\n" << RESET;
        cout << BYELLOW << "  Pilih Kategori [1-4]" << RESET;
        int kat = inputAngkaRentang(1, 4);

        cout << "\n" << BBLUE
             << "  +======================================+\n"
             << "  |" << RESET << "  [ 1 ]  Termurah ke Termahal         " << BBLUE << "|\n"
             << "  |" << RESET << "  [ 2 ]  Termahal ke Termurah         " << BBLUE << "|\n"
             << "  +======================================+\n" << RESET;
        cout << BYELLOW << "  Pilih Urutan [1-2]" << RESET;
        int urut = inputAngkaRentang(1, 2);

        animasiLoading("Memfilter produk", 2);
        filterKategoriDanSort((KategoriHewan)kat, (UrutanHarga)urut);
    }
}


//  FITUR: Proses Pembelian & Keranjang Belanja
void prosesPembelianUser(const string& username) {
    static vector<ItemKeranjang> keranjang;
    int pilihan;

    do {
        int    jmlItem   = (int)keranjang.size();
        double totalKini = 0;
        for (const auto& it : keranjang) totalKini += it.subtotal;

        cout << "\n\n"
             << BGREEN
             << "  +==================================================================+\n"
             << "  |" << RESET;
        cout << BG_GREEN << BWHITE << pusatTeks("KERANJANG BELANJA", 66) << RESET;
        cout << BGREEN << "|\n"
             << "  +------------------------------------------------------------------+\n";
        if (jmlItem > 0) {
            string info = "  " + to_string(jmlItem) + " item | Total: Rp "
                        + to_string((long long)totalKini);
            cout << "  |" << RESET << BYELLOW << info
                 << string(max(0, 66 - (int)info.size()), ' ') << RESET << BGREEN << "|\n"
                 << "  +------------------------------------------------------------------+\n";
        }
        cout << "  |" << RESET << "  " << BCYAN   << "[ 1 ]" << RESET << "  Tambah Produk ke Keranjang" << string(31,' ') << BGREEN << "|\n"
             << "  |" << RESET << "  " << BBLUE   << "[ 2 ]" << RESET << "  Lihat Isi Keranjang" << string(38,' ') << BGREEN << "|\n"
             << "  |" << RESET << "  " << BRED    << "[ 3 ]" << RESET << "  Hapus Produk dari Keranjang" << string(30,' ') << BGREEN << "|\n"
             << "  |" << RESET << "  " << BYELLOW << "[ 4 ]" << RESET << "  Bayar & Cetak Struk" << string(38,' ') << BGREEN << "|\n"
             << "  |" << RESET << "  " << BRED    << "[ 5 ]" << RESET << "  Batalkan Semua Pembelian" << string(33,' ') << BGREEN << "|\n"
             << "  |" << RESET << "  " << BWHITE  << "[ 0 ]" << RESET << "  Kembali ke Menu Pelanggan" << string(32,' ') << BGREEN << "|\n"
             << "  +==================================================================+\n" << RESET;
        cout << "\n" << BYELLOW << "  Pilih [0-5]" << RESET;
        pilihan = inputAngkaRentang(0, 5);

        // ── Tambah Produk ──
        if (pilihan == 1) {
            Produk tampil[MAX_PRODUK];
            for (int i = 0; i < jumlahProduk; i++) tampil[i] = daftarProduk[i];
            bubbleSortHarga(tampil, jumlahProduk, ASC);
            tampilkanTabelProduk(tampil, jumlahProduk);

            cout << BYELLOW << "  Masukkan ID Produk  (0 = Batal)" << RESET;
            int id = inputAngkaAman();
            if (id < 0) { cetakPesan("err", "ID tidak valid."); continue; }
            if (id == 0) continue;

            int idx = cariBinaryByID(id);
            if (idx == -1) { cetakPesan("warn", "Produk tidak ditemukan."); continue; }
            if (daftarProduk[idx].stok == 0) { cetakPesan("warn", "Stok produk habis."); continue; }

            cout << BCYAN << "\n  Produk  : " << BWHITE << daftarProduk[idx].nama    << RESET << "\n"
                 << BCYAN <<   "  Harga   : " << BGREEN << "Rp " << fixed << setprecision(0)
                 << daftarProduk[idx].harga << RESET << "\n"
                 << BCYAN <<   "  Stok    : " << BYELLOW << daftarProduk[idx].stok   << RESET << "\n";
            cout << BCYAN << "  Jumlah yang ingin dibeli" << RESET;
            int jumlah = inputAngkaAman();

            if (jumlah <= 0) { cetakPesan("err", "Jumlah harus lebih dari 0."); continue; }
            if (jumlah > daftarProduk[idx].stok) { cetakPesan("err", "Stok tidak mencukupi."); continue; }

            bool sudahAda = false;
            for (size_t i = 0; i < keranjang.size(); i++) {
                if (keranjang[i].idProduk == id) {
                    if (keranjang[i].jumlah + jumlah > daftarProduk[idx].stok) {
                        cetakPesan("err", "Total melebihi stok tersedia."); sudahAda = true; break;
                    }
                    keranjang[i].jumlah  += jumlah;
                    keranjang[i].subtotal = keranjang[i].jumlah * daftarProduk[idx].harga;
                    cetakPesan("ok", "Jumlah diperbarui di keranjang.");
                    sudahAda = true; break;
                }
            }
            if (!sudahAda) {
                ItemKeranjang item;
                item.idProduk = id;
                item.nama     = daftarProduk[idx].nama;
                item.jumlah   = jumlah;
                item.subtotal = jumlah * daftarProduk[idx].harga;
                keranjang.push_back(item);
                cetakPesan("ok", "'" + daftarProduk[idx].nama + "' ditambahkan ke keranjang.");
            }
        }
        // ── Lihat Keranjang ──
        else if (pilihan == 2) {
            tampilkanKeranjang(keranjang);
        }
        // ── Hapus dari Keranjang ──
        else if (pilihan == 3) {
            if (keranjang.empty()) { cetakPesan("warn", "Keranjang sudah kosong."); continue; }
            tampilkanKeranjang(keranjang);
            cout << BYELLOW << "  Nomor item yang akan dihapus (0 = Batal)" << RESET;
            int no = inputAngkaRentang(0, (int)keranjang.size());
            if (no == 0) continue;
            cetakPesan("ok", "'" + keranjang[no-1].nama + "' dihapus dari keranjang.");
            keranjang.erase(keranjang.begin() + (no - 1));
        }

        // ── Bayar ──
        else if (pilihan == 4) {
            if (keranjang.empty()) { cetakPesan("warn", "Keranjang masih kosong."); continue; }
            tampilkanKeranjang(keranjang);

            double total = 0;
            for (const auto& it : keranjang) total += it.subtotal;

            cout << BYELLOW << "\n  Total belanja : Rp " << fixed << setprecision(0) << total << RESET << "\n";

            // ── Pilih Metode Pembayaran ──
            cout << "\n"
                 << BYELLOW << "  +==========================================+\n"
                 << "  |" << RESET;
            cout << BG_YELLOW << "\033[30m" << pusatTeks("METODE PEMBAYARAN", 42) << RESET;
            cout << BYELLOW << "|\n"
                 << "  +------------------------------------------+\n"
                 << "  |" << RESET << "  " << BGREEN  << "[ 1 ]" << RESET
                 << "  Tunai (Cash)                     " << BYELLOW << "|\n"
                 << "  |" << RESET << "  " << BCYAN   << "[ 2 ]" << RESET
                 << "  QR Code / QRIS (buka di browser) " << BYELLOW << "|\n"
                 << "  +==========================================+\n" << RESET;
            cout << BYELLOW << "  Pilih Metode [1-2]" << RESET;
            int metodeBayar = inputAngkaRentang(1, 2);

            double uangBayar = 0.0;
            double kembalian = 0.0;
            string infoMetode;                        // buat baris metode di struk
            string ts = getTimestamp();               // ambil timestamp sekali, dipakai di QR & struk

            if (metodeBayar == 1) {
                // ── Tunai ──
                cout << BCYAN << "  Masukkan uang bayar (Rp)" << RESET;
                uangBayar = inputDoubleAman();
                if (uangBayar < total) {
                    cetakPesan("err", "Uang tidak mencukupi. Total: Rp " + to_string((long long)total));
                    continue;
                }
                kembalian  = uangBayar - total;
                infoMetode = "TUNAI";

            } else {
                // ── QR Code / QRIS ──
                // generate file HTML + buka di browser, timestamp dioper biar sama sama struk
                animasiLoading("Membuat QR Code pembayaran", 2);
                tampilkanQRCode(username, total, ts);

                cetakPesan("info", "QR Code sudah terbuka di browser!");
                cout << BCYAN << "\n"
                     << "  Scan QR tersebut menggunakan aplikasi QRIS / e-wallet kamu,\n"
                     << "  lalu konfirmasi di bawah setelah pembayaran berhasil.\n\n" << RESET;

                // konfirmasi dari user bahwa sudah bayar
                cout << BRED   << "  [ 0 ]" << RESET << "  Batalkan pembayaran\n"
                     << BGREEN << "  [ 1 ]" << RESET << "  Konfirmasi: sudah bayar via QRIS\n";
                cout << BYELLOW << "  Pilih [0/1]" << RESET;
                int konfirmQR = inputAngkaRentang(0, 1);

                if (konfirmQR == 0) {
                    cetakPesan("warn", "Pembayaran QR dibatalkan.");
                    continue;
                }
                // QR bayar pas (uangBayar = total, kembalian = 0)
                uangBayar  = total;
                kembalian  = 0;
                infoMetode = "QRIS / QR Code";
            }

            animasiLoading("Memproses pembayaran", 3);

            // Cetak struk
            cout << "\n"
                 << BWHITE << "  +=============================================+\n"
                 << "  |" << RESET;
            cout << BG_BLUE << BWHITE << pusatTeks("STRUK PEMBELIAN", 45) << RESET;
            cout << BWHITE << "|\n"
                 << "  +---------------------------------------------+\n";
            string strUser  = "  Pembeli  : " + username;
            string strTgl   = "  Tanggal  : " + ts;
            cout << "  |" << RESET << BCYAN << left << setw(45) << strUser << RESET << BWHITE << "|\n"
                 << "  |" << RESET << CYAN  << left << setw(45) << strTgl  << RESET << BWHITE << "|\n"
                 << "  +---------------------------------------------+\n";
            for (const auto& it : keranjang) {
                string nm = it.nama;
                if ((int)nm.size() > 22) nm = nm.substr(0, 21) + ".";
                string brs = "  " + nm + "  x" + to_string(it.jumlah)
                           + "  Rp" + to_string((long long)it.subtotal);
                cout << "  |" << RESET << left << setw(45) << brs << RESET << BWHITE << "|\n";
            }
            string strTotal   = "  Total     : Rp " + to_string((long long)total);
            string strBayar   = "  Dibayar   : Rp " + to_string((long long)uangBayar);
            string strKembali = "  Kembalian : Rp " + to_string((long long)kembalian);
            string strMetode  = "  Metode    : " + infoMetode;   // baris baru: metode pembayaran
            cout << "  +---------------------------------------------+\n"
                 << "  |" << RESET << BGREEN   << left << setw(45) << strTotal   << RESET << BWHITE << "|\n"
                 << "  |" << RESET << BCYAN    << left << setw(45) << strBayar   << RESET << BWHITE << "|\n"
                 << "  |" << RESET << BYELLOW  << left << setw(45) << strKembali << RESET << BWHITE << "|\n"
                 << "  |" << RESET << BMAGENTA << left << setw(45) << strMetode  << RESET << BWHITE << "|\n"
                 << "  +---------------------------------------------+\n"
                 << "  |" << RESET;
            cout << BYELLOW << pusatTeks("Terima kasih telah berbelanja!", 45) << RESET;
            cout << BWHITE << "|\n"
                 << "  +=============================================+\n" << RESET;

            // Kurangi stok & simpan riwayat
            for (const auto& item : keranjang) {
                int i = cariBinaryByID(item.idProduk);
                if (i != -1) daftarProduk[i].stok -= item.jumlah;
            }
            if (jumlahRiwayat < MAX_RIWAYAT) {
                Riwayat r;
                r.user       = username;
                r.jumlahItem = (int)keranjang.size();
                for (int i = 0; i < (int)keranjang.size(); i++) r.items[i] = keranjang[i];
                r.total     = total;
                r.tanggal   = ts;
                r.uangBayar = uangBayar;
                r.kembalian = kembalian;
                daftarRiwayat[jumlahRiwayat++] = r;
            }
            keranjang.clear();
            cetakPesan("ok", "Transaksi selesai dan tersimpan.");
            return;
        }

        // ── Batalkan Semua ──
        else if (pilihan == 5) {
            if (keranjang.empty()) { cetakPesan("warn", "Keranjang sudah kosong."); continue; }
            cout << BRED << "\n  Batalkan semua item dalam keranjang?\n" << RESET
                 << "  [ 1 ] Ya, Batalkan   [ 0 ] Tidak\n";
            cout << BYELLOW << "  Pilih [0/1]" << RESET;
            int konfirm = inputAngkaRentang(0, 1);
            if (konfirm == 1) {
                keranjang.clear();
                cetakPesan("ok", "Semua item di keranjang dibatalkan.");
            }
        }

    } while (pilihan != 0);
}


//  FITUR: Riwayat Transaksi
void tampilkanRiwayat(const string& username) {
    bool isAdmin = (username == "admin");

    cout << "\n\n";
    if (isAdmin) {
        cetakHeader("RIWAYAT PEMBELIAN SELURUH PELANGGAN", 72, BYELLOW);
    } else {
        cetakHeader("RIWAYAT PEMBELIAN  --  " + username, 72, BCYAN);
    }

    bool ada = false;
    for (int i = 0; i < jumlahRiwayat; i++) {
        if (!isAdmin && daftarRiwayat[i].user != username) continue;
        ada = true;

        cout << "\n" << BCYAN
             << "  +------------------------------------------------------------------+\n";
        string infoPembeli = "  Pembeli : " + daftarRiwayat[i].user;
        string infoTgl     = "  Tanggal : " + daftarRiwayat[i].tanggal;
        cout << "  |" << RESET << BYELLOW << left << setw(66) << infoPembeli << RESET << BCYAN << "|\n"
             << "  |" << RESET << CYAN    << left << setw(66) << infoTgl     << RESET << BCYAN << "|\n"
             << "  +------------------------------------------------------------------+\n" << RESET;

        for (int j = 0; j < daftarRiwayat[i].jumlahItem; j++) {
            const auto& it = daftarRiwayat[i].items[j];
            string nm = it.nama;
            if ((int)nm.size() > 30) nm = nm.substr(0, 30);
            string baris = "  " + nm + "  x" + to_string(it.jumlah)
                         + "  =>  Rp " + to_string((long long)it.subtotal);
            cout << BCYAN << "  |" << RESET << left << setw(66) << baris << BCYAN << "|\n" << RESET;
        }

        cout << BCYAN << "  +------------------------------------------------------------------+\n" << RESET;
        string strTotal   = "  Total Belanja : Rp " + to_string((long long)daftarRiwayat[i].total);
        string strBayar   = "  Uang Dibayar  : Rp " + to_string((long long)daftarRiwayat[i].uangBayar);
        string strKembali = "  Kembalian     : Rp " + to_string((long long)daftarRiwayat[i].kembalian);
        cout << BCYAN << "  |" << RESET << BGREEN  << left << setw(66) << strTotal   << RESET << BCYAN << "|\n"
             <<         "  |" << RESET << BCYAN    << left << setw(66) << strBayar   << RESET << BCYAN << "|\n"
             <<         "  |" << RESET << BYELLOW  << left << setw(66) << strKembali << RESET << BCYAN << "|\n"
             << "  +------------------------------------------------------------------+\n" << RESET;
    }

    if (!ada) {
        cout << BYELLOW << "  " << pusatTeks("Belum ada riwayat pembelian.", 66) << RESET << "\n";
        cetakGarisH(72, '=', BCYAN);
    }
}


//  FITUR: Menu Pelanggan
void menuUser(const string& username) {
    int pilihan;
    do {
        cout << "\n\n"
             << BCYAN
             << "  +======================================================================+\n"
             << "  |" << RESET;
        cout << BG_CYAN << "\033[30m" << pusatTeks("MENU PELANGGAN  --  Hai, " + username + "!", 70) << RESET;
        cout << BCYAN << "|\n"
             << "  +----------------------------------------------------------------------+\n"
             << "  |" << RESET << "  " << BBLUE    << "[ 1 ]" << RESET << "  Lihat Semua Produk" << string(43,' ') << BCYAN << "|\n"
             << "  |" << RESET << "  " << BMAGENTA << "[ 2 ]" << RESET << "  Cari & Filter Produk" << string(41,' ') << BCYAN << "|\n"
             << "  |" << RESET << "  " << BGREEN   << "[ 3 ]" << RESET << "  Keranjang Belanja" << string(44,' ') << BCYAN << "|\n"
             << "  |" << RESET << "  " << BYELLOW  << "[ 4 ]" << RESET << "  Riwayat Pembelian Saya" << string(39,' ') << BCYAN << "|\n"
             << "  +----------------------------------------------------------------------+\n"
             << "  |" << RESET << "  " << BRED     << "[ 0 ]" << RESET << "  Keluar dari Akun" << string(45,' ') << BCYAN << "|\n"
             << "  +======================================================================+\n" << RESET;

        cout << "\n" << BYELLOW << "  Pilih Menu [0-4]" << RESET;
        pilihan = inputAngkaRentang(0, 4);

        try {
            switch (pilihan) {
                case 1: {
                    Produk temp[MAX_PRODUK];
                    copy(daftarProduk, daftarProduk + jumlahProduk, temp);
                    animasiLoading("Memuat daftar produk", 2);
                    clearLayar();
                    bubbleSortHarga(temp, jumlahProduk, ASC);
                    tampilkanTabelProduk(temp, jumlahProduk);
                    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali ke menu..." << RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                case 2:
                    clearLayar();
                    subMenuCariFilterUser();
                    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali ke menu..." << RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                case 3:
                    clearLayar();
                    prosesPembelianUser(username);
                    break;
                case 4:
                    animasiLoading("Memuat riwayat", 2);
                    clearLayar();
                    tampilkanRiwayat(username);
                    cout << "\n" << BYELLOW << "  Tekan [Enter] untuk kembali ke menu..." << RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                case 0:
                    animasiLoading("Keluar dari akun", 2);
                    clearLayar();
                    cetakPesan("info", "Berhasil keluar dari akun " + username + ". Sampai jumpa!");
                    break;
            }
        } catch (const exception& e) {
            cetakPesan("err", e.what());
        }
    } while (pilihan != 0);
}