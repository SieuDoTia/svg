// soDo_LichSuTraiDat_SVG.c
// Sơ đồ lịch sử Trái Đất/lịch sử địa chất
// Khởi động 2023.06.01
// Phiên bản 2023.06.21
// Phạm vi công cộng

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ---- khổ chuẩn cho nửa A0 ngang
#define kBE_RONG__KHO 4494
#define kBE_CAO__KHO  1589

#define kLE_TRAI 150.0f  // cho số trục y
#define kLE_PHAI 150.0f
#define kLE_DUOI 160.0f  // cho số trục x
#define kLE_TREN 250.0f  // cho tiêu đề

#define kGIUA_NGANG 5.0f
#define kGIUA_DOC   5.0f

#define kSAI  0
#define kDUNG 1

// rộng: 2 + 14 + 10 + 6 = 32
// cao: 7

#pragma mark ==== SVG
void ghiDauSVG( FILE *tep, unsigned int beRong, unsigned int beCao ) {    // ---- đầu tập tin SVG
   
   fprintf( tep, "<svg version=\"1.1\" width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n\n", beRong, beCao );
}

// Vẽ hình chữ nhật tại góc trái dưới tại (gocX; gocY)
void chuNhat( FILE *tep, float gocX, float gocY, float beRong, float beCao, unsigned char toDay, unsigned int mauToDay, float doDucToDay,
             unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet ) {
   
   fprintf( tep, "<rect x=\"%5.1f\" y=\"%5.1f\" width=\"%5.1f\" height=\"%5.1f\" ", gocX, gocY, beRong, beCao );
   if( toDay )
      fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
   else
      fprintf( tep, "fill=\"none\" " );
   if( net ) {
      fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
      fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
   }
   else {
      fprintf( tep, "stroke=\"none\" " );
   }
   
   fprintf( tep, "/>\n\n" );
}

void vanBan_ngang( FILE *tep, char *xau, float viTriX, float viTriY, char *giaDinhPhong, float coKichPhong, char *beDayPhong,
                  unsigned char toDay, unsigned int mauToDay, float doDucToDay, unsigned char net, float beRongNet, unsigned int mauNet, float doDucNet, char *canhHang ) {
   
   fprintf( tep, "<text x=\"%5.1f\" y=\"%5.1f\" font-family=\"%s\" font-size=\"%5.1f\" font-weight=\"%s\" writing-mode=\"lr\" ", viTriX, viTriY, giaDinhPhong, coKichPhong, beDayPhong );
   
   fprintf( tep, "text-anchor=\"%s\" ", canhHang );   // start middle  end
   
   if( toDay )
      fprintf( tep, "fill=\"#%06X\" fill-opacity=\"%4.2f\" ", mauToDay, doDucToDay );
   else
      fprintf( tep, "fill=\"none\" " );
   if( net ) {
      fprintf( tep, "stroke=\"#%06X\" stroke-opacity=\"%4.2f\" ", mauNet, doDucNet );
      fprintf( tep, "stroke-width=\"%5.1f\" ", beRongNet );
   }
   else {
      fprintf( tep, "stroke=\"none\" " );
   }
   fprintf( tep, ">\n" );
   fprintf( tep, " %s </text>\n\n", xau );
}

// Vẽ một nét giữa hai điểm
void duong( FILE *tep, float diemX0, float diemY0, float diemX1, float diemY1, float beRongNet, unsigned int mauNet, float doDucNet ) {

   fprintf( tep, "<line x1=\"%5.1f\" y1=\"%5.1f\" x2=\"%5.1f\" y2=\"%5.1f\" stroke=\"#%06X\" stroke-opacity=\"%4.2f\" stroke-width=\"%5.1f\" />\n",
           diemX0, diemY0, diemX1, diemY1, mauNet, doDucNet, beRongNet );
}



#define kSO_LUONG__NGUYEN_TO 118  //  số lượng nguyên tốcho vẽ bảng tuền hoàn

#define kKHONG_BIET '-'


// ----- Nguyên Tố
typedef struct {
   unsigned char so;       // số nguyên tố
   char ten[32];           // tên
   char kyHieu[4];         // ký hiệu
   char tenTrung[5];       // tên tiếng Trung Quốc
   float khoiLuong;        // khối lượng nguyên tử chuẩn
   char soOxyHoa[32];      // số oxy hóa phổ biến
   float tiTrong;          // tỉ trọng (kg/m3 tại 273 K)
   float nhietDoChay;      // nhiệt độ chảy (K)
   float nhietDoSoi;       // nhiệt độ sôi (K)
   float banKinh;          // pm (Van Der Waals)
   char cauHinhElecton[32]; // cầu hình electron
   float nhietDung;        // nhiệt dunng(kJ/(kmol K)
   float doAmDien;         // độ điện âm
   unsigned char loai;     // rắn, lỏng, khí
   unsigned char cauTrucTinhThe;  // cấu trúc tinh thể
   float nangLuongIonHoa1; // năng lượng ion hóa 1 (kJ/mol)
   float nangLuongIonHoa2; // năng lượng ion hóa 2 (kJ/kmol)
   unsigned char dongViOnDinh[11];  // đồng vị ổ định
} NguyenTo;

// ---- danh sách nguyên tố
enum {
   HIDRO,   // Hiđrô
   HELI,    // Heli
   
   LITI,   // Liti
   BERI,    // Beri
   BO,      // Bo
   CACBON,  // Cacbon
   NITO,    // Nitơ
   OXI,     // Oxi
   FLO,     // Flo,
   NEON,    // Neon
   
   NATRI,   // Natri
   MAGIE,   // Magie
   NHOM,    // Nhôm
   SILIC,   // Silic
   PHOTPHO, // Photpho
   LUU_HUYNH, // Lưu Huỳnh
   CLO,     // Clo
   ARGON,   // Argon
   
   KALI,    // Kali
   CANXI,   // Canxi
   SCANDI,  // Scanđi
   TITAN,  // Titan
   VANADI,  // Vanđi
   CROM,    // Crom
   MANGAN,  // Mangan
   SAT,     // Sắt
   COBAN,   // Coban
   NICKEN,  // Nicken
   DONG,    // Đồng
   KEM,     // Kẽm
   GALI,    // Gali
   GEMANI,  // Gemani
   ASEN,    // Asen
   SELENI,  // Seleni
   BROM,    // Brom
   KRYPTON, // Krypton
   
   RUBIDI,   // Rubiđi
   STRONTI,  // Stronti
   YTRI,     // Ytri
   ZIRCONI,  // Zirconi
   NIOBI,    // Niobi
   MOLIPDEN, // Molipđen
   TECNEXI,  // Tecnexi
   RUTENI,   // Ruteni
   RODI,     // Rôđi
   PALADI,   // Palađi
   BAC,      // Bạc
   CADIMI,   // Cađimi
   INDI,     // Indi
   THIEC,    // Thiếc
   ANTIMON,  // Antimon
   TELU,     // Telu
   IOT,      // Iot
   XENON,    // Xenon
   
   XESI,     // Xesi
   BARI,     // Bari
   LANTAN,   // Lantan
   XERI,     // Xeri
   PRAZEODIM,// Prazeođim
   NEODYMI,  // Neođim
   PROMETI,  // Prometi
   SAMARI,   // Samari
   EUROPI,   // Europi
   GADOLINI, // Gađolini
   TEBI,     // Terbi
   DIPROZI,  // Điprozi
   HONMI,    // Honmi
   ERBI,     // Erbi
   TULI,     // Tuli
   YTECBI,   // Ytecbi
   LUTEXI,   // Lutexi
   HAFINI,   // Hafini
   TANTAN,   // Tantan
   VONFAM,   // Vonfam
   RENI,     // Reni
   OSIMI,    // Osimi
   IRIDI,    // Iriđi
   PLATIN,   // Platin
   VANG,     // Vàng
   THUY_NGAN,// Thủy Ngan
   TALI,     // Tali
   CHI,      // Chì
   BITMUT,   // Bitmut
   POLONI,   // Poloni
   ASTATIN,  // Astatin
   RADON,    // Rađon

   FRANXI,   // Franxi
   RADI,     // Rađi
   ACTINI,   // Actini
   THORI,    // Thori
   PROTACTINI, // Protactini
   URANI,    // Urani
   NEPTUNI,  // Neptuni
   PLUTONI,  // Plutoni
   AMERIXI,  // Amerixi
   CURI,     // Curi
   BECKELI,  // Beckeli
   CALIFORNI, // Californi
   ENSTENI,  // Esteni
   FECMI,     // Fecmi
   MENDELEVI, // Menđelevi
   NOBELI,    // Nobeli
   LORENXI,   // Lorenxi
   ROZOFODI,  // Rozofodi
   DUBNI,     // Đubni
   SIBOGI,    // Siboqi
   BORI,      // Bori
   HASI,      // Hasi
   MEITNERI,  // Méitner©i
   DAMASTATI, // Đamastati
   REONTGENI, // Reontgeni
   COPENIXI,  // Copernixi
   NIHONI,    // Nihoni
   FLEROVI,   // Flerovi
   MOSCOVI,   // Moscovi
   LIVEMORI,  // Livemori
   TENEXIN,   // Tenexin
   OGANESON   // Ôganeson
};

enum {
   KHI,   // khí
   LONG,  // lỏng
   RAN    // rắn
};

enum {
   LAP_PHUONG,         // lập phương
   LAP_PHUONG_TAM_MAT, // lập phương tâm mặt
   LAP_PHUONG_TAM_KHOI,// lập phương tâm khối
   LAP_PHUONG_KIM_CUONG,// lập phương kim cương
   LUC_PHUONG,         // lục phương
   TRUC_THOI,          // trực thoi
   DON_NGHIENG,        // đơn nhiêng
   BA_NGHIENG,         // ba nghiêng
   BA_PHUONG,          // ba phương
   VO_DANG,            // vô dạng
};

#pragma mark ==== Chuẩn Bị Thôn Tin Nguyên Tố
void chuanBiThongTinNguyenTo( NguyenTo *mangNguyenTo ) {
   
   // ---- Hiđrô
   NguyenTo *nguyenTo = &(mangNguyenTo[HIDRO]);
   nguyenTo->so = 1;
   strcpy( nguyenTo->ten, "Hiđrô" );
   strcpy( nguyenTo->kyHieu, "H" );
   strcpy( nguyenTo->tenTrung, "氢" );
   nguyenTo->khoiLuong = 1.0079f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "1s¹" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 14.01;
   nguyenTo->nhietDoSoi = 20.28;
   nguyenTo->tiTrong = 0.08988;
   nguyenTo->nhietDung = 28.836f;
   nguyenTo->doAmDien = 2.20f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 1312.0f;
   nguyenTo->nangLuongIonHoa2 = 0.0f;
   nguyenTo->banKinh = 120;
   nguyenTo->dongViOnDinh[0] = 1;
   nguyenTo->dongViOnDinh[1] = 2;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Heli
   nguyenTo = &(mangNguyenTo[HELI]);
   nguyenTo->so = 2;
   strcpy( nguyenTo->ten, "Heli" );
   strcpy( nguyenTo->kyHieu, "He" );
   strcpy( nguyenTo->tenTrung, "氦" );
   nguyenTo->khoiLuong = 4.0026f;
   strcpy( nguyenTo->soOxyHoa, "0" );
   strcpy( nguyenTo->cauHinhElecton, "1s²" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 0.95;
   nguyenTo->nhietDoSoi = 4.22;
   nguyenTo->tiTrong = 0.1786;
   nguyenTo->nhietDung = 20.786f;
   nguyenTo->doAmDien = 0.0f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 2372.3f;
   nguyenTo->nangLuongIonHoa2 = 5250.5f;
   nguyenTo->banKinh = 140;
   nguyenTo->dongViOnDinh[0] = 3;
   nguyenTo->dongViOnDinh[1] = 4;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Liti
   nguyenTo = &(mangNguyenTo[LITI]);
   nguyenTo->so = 3;
   strcpy( nguyenTo->ten, "Liti" );
   strcpy( nguyenTo->kyHieu, "Li" );
   strcpy( nguyenTo->tenTrung, "锂" );
   nguyenTo->khoiLuong = 6.941f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 453.65;
   nguyenTo->nhietDoSoi = 1603;
   nguyenTo->tiTrong = 534;
   nguyenTo->nhietDung = 24.860f;
   nguyenTo->doAmDien = 0.98f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 520.2f;
   nguyenTo->nangLuongIonHoa2 = 7298.1f;
   nguyenTo->banKinh = 182;
   nguyenTo->dongViOnDinh[0] = 6;
   nguyenTo->dongViOnDinh[1] = 7;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Beri
   nguyenTo = &(mangNguyenTo[BERI]);
   nguyenTo->so = 4;
   strcpy( nguyenTo->ten, "Beri" );
   strcpy( nguyenTo->kyHieu, "Be" );
   strcpy( nguyenTo->tenTrung, "铍" );
   nguyenTo->khoiLuong = 9.0122f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1560;
   nguyenTo->nhietDoSoi = 2742;
   nguyenTo->tiTrong = 1850;
   nguyenTo->nhietDung = 16.443f;
   nguyenTo->doAmDien = 1.57f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 899.5f;
   nguyenTo->nangLuongIonHoa2 = 1757.1f;
   nguyenTo->banKinh = 153;
   nguyenTo->dongViOnDinh[0] = 9;
   nguyenTo->dongViOnDinh[1] = 0;
   
   // ---- Bo
   nguyenTo = &(mangNguyenTo[BO]);
   nguyenTo->so = 5;
   strcpy( nguyenTo->ten, "Bo" );
   strcpy( nguyenTo->kyHieu, "B" );
   strcpy( nguyenTo->tenTrung, "硼" );
   nguyenTo->khoiLuong = 10.8117f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²2p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2349;
   nguyenTo->nhietDoSoi = 4200;
   nguyenTo->tiTrong = 2080;  // <---- nhiệt độ chảy
   nguyenTo->nhietDung = 11.087f;
   nguyenTo->doAmDien = 2.04f;
   nguyenTo->cauTrucTinhThe = BA_PHUONG;  // ???
   nguyenTo->nangLuongIonHoa1 = 800.6f;
   nguyenTo->nangLuongIonHoa2 = 2427.1f;
   nguyenTo->banKinh = 192;
   nguyenTo->dongViOnDinh[0] = 10;
   nguyenTo->dongViOnDinh[1] = 11;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Cacbon
   nguyenTo = &(mangNguyenTo[CACBON]);
   nguyenTo->so = 6;
   strcpy( nguyenTo->ten, "Cacbon" );
   strcpy( nguyenTo->kyHieu, "C" );
   strcpy( nguyenTo->tenTrung, "碳" );
   nguyenTo->khoiLuong = 12.0108f;
   strcpy( nguyenTo->soOxyHoa, "+4, –4" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²2p²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 0.0f;  // không chảy
   nguyenTo->nhietDoSoi = 3915;
   nguyenTo->tiTrong = 2267;
   nguyenTo->nhietDung = 8.517f;
   nguyenTo->doAmDien = 2.55f;
   nguyenTo->cauTrucTinhThe = VO_DANG;
   nguyenTo->nangLuongIonHoa1 = 1086.5f;
   nguyenTo->nangLuongIonHoa2 = 2352.6f;
   nguyenTo->banKinh = 170;
   nguyenTo->dongViOnDinh[0] = 12;
   nguyenTo->dongViOnDinh[1] = 13;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Nitơ
   nguyenTo = &(mangNguyenTo[NITO]);
   nguyenTo->so = 7;
   strcpy( nguyenTo->ten, "Nitơ" );
   strcpy( nguyenTo->kyHieu, "N" );
   strcpy( nguyenTo->tenTrung, "氮" );
   nguyenTo->khoiLuong = 14.0067f;
   strcpy( nguyenTo->soOxyHoa, "–3" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²2p³" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 63.15f;
   nguyenTo->nhietDoSoi = 77.36;
   nguyenTo->tiTrong = 1.251;
   nguyenTo->nhietDung = 29.124f;
   nguyenTo->doAmDien = 3.04f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 1402.3f;
   nguyenTo->nangLuongIonHoa2 = 2856.0f;
   nguyenTo->banKinh = 155;
   nguyenTo->dongViOnDinh[0] = 14;
   nguyenTo->dongViOnDinh[1] = 15;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Oxi
   nguyenTo = &(mangNguyenTo[OXI]);
   nguyenTo->so = 8;
   strcpy( nguyenTo->ten, "Oxi" );
   strcpy( nguyenTo->kyHieu, "O" );
   strcpy( nguyenTo->tenTrung, "氧" );
   nguyenTo->khoiLuong = 15.9994f;
   strcpy( nguyenTo->soOxyHoa, "–2" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²2p⁴" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 54.36f;
   nguyenTo->nhietDoSoi = 90.20;
   nguyenTo->tiTrong = 1.429;
   nguyenTo->nhietDung = 29.378f;
   nguyenTo->doAmDien = 3.44f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 1313.9f;
   nguyenTo->nangLuongIonHoa2 = 3388.3f;
   nguyenTo->banKinh = 152;
   nguyenTo->dongViOnDinh[0] = 16;
   nguyenTo->dongViOnDinh[1] = 17;
   nguyenTo->dongViOnDinh[2] = 18;
   nguyenTo->dongViOnDinh[3] = 0;
   
   // ---- Flo
   nguyenTo = &(mangNguyenTo[FLO]);
   nguyenTo->so = 9;
   strcpy( nguyenTo->ten, "Flo" );
   strcpy( nguyenTo->kyHieu, "F" );
   strcpy( nguyenTo->tenTrung, "氟" );
   nguyenTo->khoiLuong = 18.9984f;
   strcpy( nguyenTo->soOxyHoa, "–1" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²2p⁵" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 53.48f;
   nguyenTo->nhietDoSoi = 85.03;
   nguyenTo->tiTrong = 1.696;
   nguyenTo->nhietDung = 31.0f;
   nguyenTo->doAmDien = 3.98f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 1681.0f;
   nguyenTo->nangLuongIonHoa2 = 3374.0f;
   nguyenTo->banKinh = 135;
   nguyenTo->dongViOnDinh[0] = 19;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Neon
   nguyenTo = &(mangNguyenTo[NEON]);
   nguyenTo->so = 10;
   strcpy( nguyenTo->ten, "Neon" );
   strcpy( nguyenTo->kyHieu, "Ne" );
   strcpy( nguyenTo->tenTrung, "氖" );
   nguyenTo->khoiLuong = 20.1798f;
   strcpy( nguyenTo->soOxyHoa, "0" );
   strcpy( nguyenTo->cauHinhElecton, "1s²2s²2p⁶" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 24.56f;
   nguyenTo->nhietDoSoi = 27.07;
   nguyenTo->tiTrong = 0.9002;
   nguyenTo->nhietDung = 20.786;
   nguyenTo->doAmDien = 0.0f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 2080.7f;
   nguyenTo->nangLuongIonHoa2 = 3952.3f;
   nguyenTo->banKinh = 154;
   nguyenTo->dongViOnDinh[0] = 20;
   nguyenTo->dongViOnDinh[1] = 21;
   nguyenTo->dongViOnDinh[2] = 22;
   nguyenTo->dongViOnDinh[3] = 0;
   
   // ---- Natri
   nguyenTo = &(mangNguyenTo[NATRI]);
   nguyenTo->so = 11;
   strcpy( nguyenTo->ten, "Natri" );
   strcpy( nguyenTo->kyHieu, "Na" );
   strcpy( nguyenTo->tenTrung, "钠" );
   nguyenTo->khoiLuong = 22.9897f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 370.87f;
   nguyenTo->nhietDoSoi = 1156;
   nguyenTo->tiTrong = 968;
   nguyenTo->nhietDung = 28.230f;
   nguyenTo->doAmDien = 0.93f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 495.8f;
   nguyenTo->nangLuongIonHoa2 = 4562.0f;
   nguyenTo->banKinh = 227;
   nguyenTo->dongViOnDinh[0] = 23;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Magie
   nguyenTo = &(mangNguyenTo[MAGIE]);
   nguyenTo->so = 12;
   strcpy( nguyenTo->ten, "Magie" );
   strcpy( nguyenTo->kyHieu, "Mg" );
   strcpy( nguyenTo->tenTrung, "镁" );
   nguyenTo->khoiLuong = 24.3051f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 923;
   nguyenTo->nhietDoSoi = 1363;
   nguyenTo->tiTrong = 1738;
   nguyenTo->nhietDung = 24.869;
   nguyenTo->doAmDien = 1.31f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 737.7f;
   nguyenTo->nangLuongIonHoa2 = 1450.7f;
   nguyenTo->banKinh = 173;
   nguyenTo->dongViOnDinh[0] = 24;
   nguyenTo->dongViOnDinh[1] = 25;
   nguyenTo->dongViOnDinh[2] = 26;
   nguyenTo->dongViOnDinh[3] = 0;

   // ---- Nhôm
   nguyenTo = &(mangNguyenTo[NHOM]);
   nguyenTo->so = 13;
   strcpy( nguyenTo->ten, "Nhôm" );
   strcpy( nguyenTo->kyHieu, "Al" );
   strcpy( nguyenTo->tenTrung, "铝" );
   nguyenTo->khoiLuong = 26.9815f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²3p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 933.47f;
   nguyenTo->nhietDoSoi = 2792;
   nguyenTo->tiTrong = 2700;
   nguyenTo->nhietDung = 24.200f;
   nguyenTo->doAmDien = 1.61f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 577.5f;
   nguyenTo->nangLuongIonHoa2 = 1816.7f;
   nguyenTo->banKinh = 184;
   nguyenTo->dongViOnDinh[0] = 27;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Silic
   nguyenTo = &(mangNguyenTo[SILIC]);
   nguyenTo->so = 14;
   strcpy( nguyenTo->ten, "Silic" );
   strcpy( nguyenTo->kyHieu, "Si" );
   strcpy( nguyenTo->tenTrung, "硅" );
   nguyenTo->khoiLuong = 28.0855f;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²3p²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1687;
   nguyenTo->nhietDoSoi = 3538;
   nguyenTo->tiTrong = 2329;
   nguyenTo->nhietDung = 19.789f;
   nguyenTo->doAmDien = 1.90f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 786.5f;
   nguyenTo->nangLuongIonHoa2 = 1577.1f;
   nguyenTo->banKinh = 210;
   nguyenTo->dongViOnDinh[0] = 28;
   nguyenTo->dongViOnDinh[1] = 29;
   nguyenTo->dongViOnDinh[2] = 30;
   nguyenTo->dongViOnDinh[3] = 0;
   
   // ---- Photpho
   nguyenTo = &(mangNguyenTo[PHOTPHO]);
   nguyenTo->so = 15;
   strcpy( nguyenTo->ten, "Photpho" );
   strcpy( nguyenTo->kyHieu, "P" );
   strcpy( nguyenTo->tenTrung, "磷" );
   nguyenTo->khoiLuong = 30.9737f;
   strcpy( nguyenTo->soOxyHoa, "+5,–3" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²3p³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 0;
   nguyenTo->nhietDoSoi = 170+273;
   nguyenTo->tiTrong = 2300;
   nguyenTo->nhietDung = 23.824f;
   nguyenTo->doAmDien = 2.19f;
   nguyenTo->cauTrucTinhThe = VO_DANG;
   nguyenTo->nangLuongIonHoa1 = 1011.8f;
   nguyenTo->nangLuongIonHoa2 = 1907;
   nguyenTo->banKinh = 180;
   nguyenTo->dongViOnDinh[0] = 31;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Lưu Huỳnh
   nguyenTo = &(mangNguyenTo[LUU_HUYNH]);
   nguyenTo->so = 16;
   strcpy( nguyenTo->ten, "Lưu Huỳnh" );
   strcpy( nguyenTo->kyHieu, "S" );
   strcpy( nguyenTo->tenTrung, "硫" );
   nguyenTo->khoiLuong = 32.0655f;
   strcpy( nguyenTo->soOxyHoa, "–2" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²3p⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 388.36;
   nguyenTo->nhietDoSoi = 717.8;
   nguyenTo->tiTrong = 2070;
   nguyenTo->nhietDung = 22.75f;
   nguyenTo->doAmDien = 2.58f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 999.6f;
   nguyenTo->nangLuongIonHoa2 = 2252;
   nguyenTo->banKinh = 180;
   nguyenTo->dongViOnDinh[0] = 32;
   nguyenTo->dongViOnDinh[1] = 33;
   nguyenTo->dongViOnDinh[2] = 34;
   nguyenTo->dongViOnDinh[3] = 0;

   // ---- Clo
   nguyenTo = &(mangNguyenTo[CLO]);
   nguyenTo->so = 17;
   strcpy( nguyenTo->ten, "Clo" );
   strcpy( nguyenTo->kyHieu, "Cl" );
   strcpy( nguyenTo->tenTrung, "氯" );
   nguyenTo->khoiLuong = 35.4532f;
   strcpy( nguyenTo->soOxyHoa, "–1" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²3p⁵" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 171.6f;
   nguyenTo->nhietDoSoi = 239.11;
   nguyenTo->tiTrong = 3.200;  // <---- ?????
   nguyenTo->nhietDung = 33.949f;
   nguyenTo->doAmDien = 3.16f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 1251.2f;
   nguyenTo->nangLuongIonHoa2 = 2298;
   nguyenTo->banKinh = 100;  // <----- ????
   nguyenTo->dongViOnDinh[0] = 35;
   nguyenTo->dongViOnDinh[1] = 36;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Argon
   nguyenTo = &(mangNguyenTo[ARGON]);
   nguyenTo->so = 18;
   strcpy( nguyenTo->ten, "Argon" );
   strcpy( nguyenTo->kyHieu, "Ar" );
   strcpy( nguyenTo->tenTrung, "氩" );
   nguyenTo->khoiLuong = 39.9481f;
   strcpy( nguyenTo->soOxyHoa, "0" );
   strcpy( nguyenTo->cauHinhElecton, "[Ne] 3s²3p⁶" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 83.8;
   nguyenTo->nhietDoSoi = 87.3;
   nguyenTo->tiTrong = 1.784f;
   nguyenTo->nhietDung = 20.786f;
   nguyenTo->doAmDien = 0.0f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 1520.6f;
   nguyenTo->nangLuongIonHoa2 = 2665.8f;
   nguyenTo->banKinh = 188;
   nguyenTo->dongViOnDinh[0] = 36;
   nguyenTo->dongViOnDinh[1] = 38;
   nguyenTo->dongViOnDinh[2] = 40;
   nguyenTo->dongViOnDinh[3] = 0;

   // ---- Kali
   nguyenTo = &(mangNguyenTo[KALI]);
   nguyenTo->so = 19;
   strcpy( nguyenTo->ten, "Kali" );
   strcpy( nguyenTo->kyHieu, "K" );
   strcpy( nguyenTo->tenTrung, "钾" );
   nguyenTo->khoiLuong = 39.0983f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 336.53f;
   nguyenTo->nhietDoSoi = 1032;
   nguyenTo->tiTrong = 862;
   nguyenTo->nhietDung = 29.6f;
   nguyenTo->doAmDien = 0.82f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 418.8f;
   nguyenTo->nangLuongIonHoa2 = 3052;
   nguyenTo->banKinh = 275;
   nguyenTo->dongViOnDinh[0] = 39;
   nguyenTo->dongViOnDinh[1] = 40;
   nguyenTo->dongViOnDinh[2] = 41;
   nguyenTo->dongViOnDinh[3] = 0;
   
   // ---- Canxi
   nguyenTo = &(mangNguyenTo[CANXI]);
   nguyenTo->so = 20;
   strcpy( nguyenTo->ten, "Canxi" );
   strcpy( nguyenTo->kyHieu, "Ca" );
   strcpy( nguyenTo->tenTrung, "钙" );
   nguyenTo->khoiLuong = 40.0784f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1115;
   nguyenTo->nhietDoSoi = 1757;
   nguyenTo->tiTrong = 1550;
   nguyenTo->nhietDung = 25.929f;
   nguyenTo->doAmDien = 1.00f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 589.8f;
   nguyenTo->nangLuongIonHoa2 = 1145.4f;
   nguyenTo->banKinh = 231;
   nguyenTo->dongViOnDinh[0] = 40;
   nguyenTo->dongViOnDinh[1] = 42;
   nguyenTo->dongViOnDinh[2] = 43;
   nguyenTo->dongViOnDinh[3] = 44;
   nguyenTo->dongViOnDinh[4] = 46;
   nguyenTo->dongViOnDinh[5] = 48;
   nguyenTo->dongViOnDinh[6] = 0;
   
   // ---- Scanđi
   nguyenTo = &(mangNguyenTo[SCANDI]);
   nguyenTo->so = 21;
   strcpy( nguyenTo->ten, "Scanđi" );
   strcpy( nguyenTo->kyHieu, "Sc" );
   strcpy( nguyenTo->tenTrung, "钪" );
   nguyenTo->khoiLuong = 44.9559f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1814;
   nguyenTo->nhietDoSoi = 3109;
   nguyenTo->tiTrong = 2985;
   nguyenTo->nhietDung = 25.52f;
   nguyenTo->doAmDien = 1.36f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 633.1f;
   nguyenTo->nangLuongIonHoa2 = 1235.0f;
   nguyenTo->banKinh = 211;
   nguyenTo->dongViOnDinh[0] = 45;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Titan
   nguyenTo = &(mangNguyenTo[TITAN]);
   nguyenTo->so = 22;
   strcpy( nguyenTo->ten, "Titan" );
   strcpy( nguyenTo->kyHieu, "Ti" );
   strcpy( nguyenTo->tenTrung, "钛" );
   nguyenTo->khoiLuong = 47.8671f;
   strcpy( nguyenTo->soOxyHoa, "+3,+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1941;
   nguyenTo->nhietDoSoi = 3560;
   nguyenTo->tiTrong = 4506;
   nguyenTo->nhietDung = 25.06f;
   nguyenTo->doAmDien = 1.54f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 658.8f;
   nguyenTo->nangLuongIonHoa2 = 1309.8f;
   nguyenTo->banKinh = 147;  // <------ ???
   nguyenTo->dongViOnDinh[0] = 46;
   nguyenTo->dongViOnDinh[1] = 47;
   nguyenTo->dongViOnDinh[2] = 48;
   nguyenTo->dongViOnDinh[3] = 49;
   nguyenTo->dongViOnDinh[4] = 50;
   nguyenTo->dongViOnDinh[5] = 0;
   
   // ---- Vanađi
   nguyenTo = &(mangNguyenTo[VANADI]);
   nguyenTo->so = 23;
   strcpy( nguyenTo->ten, "Vanađi" );
   strcpy( nguyenTo->kyHieu, "V" );
   strcpy( nguyenTo->tenTrung, "钒" );
   nguyenTo->khoiLuong = 50.9415f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+4,+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2183;
   nguyenTo->nhietDoSoi = 3680;
   nguyenTo->tiTrong = 6000;
   nguyenTo->nhietDung = 24.89f;
   nguyenTo->doAmDien = 1.63f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 650.9f;
   nguyenTo->nangLuongIonHoa2 = 1414;
   nguyenTo->banKinh = 134;  // ???
   nguyenTo->dongViOnDinh[0] = 50;
   nguyenTo->dongViOnDinh[1] = 51;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Crom
   nguyenTo = &(mangNguyenTo[CROM]);
   nguyenTo->so = 24;
   strcpy( nguyenTo->ten, "Crom" );
   strcpy( nguyenTo->kyHieu, "Cr" );
   strcpy( nguyenTo->tenTrung, "铬" );
   nguyenTo->khoiLuong = 50.9415f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s¹3d⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2180;
   nguyenTo->nhietDoSoi = 2944;
   nguyenTo->tiTrong = 7190;
   nguyenTo->nhietDung = 23.35f;
   nguyenTo->doAmDien = 1.66f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 650.9f;
   nguyenTo->nangLuongIonHoa2 = 1414;
   nguyenTo->banKinh = 128;  // ???
   nguyenTo->dongViOnDinh[0] = 50;
   nguyenTo->dongViOnDinh[1] = 52;
   nguyenTo->dongViOnDinh[2] = 53;
   nguyenTo->dongViOnDinh[3] = 54;
   nguyenTo->dongViOnDinh[4] = 0;

   // ---- Mangan
   nguyenTo = &(mangNguyenTo[MANGAN]);
   nguyenTo->so = 25;
   strcpy( nguyenTo->ten, "Mangan" );
   strcpy( nguyenTo->kyHieu, "Mn" );
   strcpy( nguyenTo->tenTrung, "锰" );
   nguyenTo->khoiLuong = 54.9380f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+4,+6,+7" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1519;
   nguyenTo->nhietDoSoi = 2334;
   nguyenTo->tiTrong = 7210;
   nguyenTo->nhietDung = 26.32f;
   nguyenTo->doAmDien = 1.55f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 717.3f;
   nguyenTo->nangLuongIonHoa2 = 1509.0f;
   nguyenTo->banKinh = 127;  // ???
   nguyenTo->dongViOnDinh[0] = 55;
   nguyenTo->dongViOnDinh[1] = 0;
   
   // ---- Sắt
   nguyenTo = &(mangNguyenTo[SAT]);
   nguyenTo->so = 26;
   strcpy( nguyenTo->ten, "Sắt" );
   strcpy( nguyenTo->kyHieu, "Fe" );
   strcpy( nguyenTo->tenTrung, "铁" );
   nguyenTo->khoiLuong = 55.8452f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d⁶" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1811;
   nguyenTo->nhietDoSoi = 3134;
   nguyenTo->tiTrong = 7874;
   nguyenTo->nhietDung = 25.10f;
   nguyenTo->doAmDien = 1.83f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 762.5f;
   nguyenTo->nangLuongIonHoa2 = 1561.9f;
   nguyenTo->banKinh = 126;  // ???
   nguyenTo->dongViOnDinh[0] = 54;
   nguyenTo->dongViOnDinh[1] = 56;
   nguyenTo->dongViOnDinh[2] = 57;
   nguyenTo->dongViOnDinh[3] = 58;
   nguyenTo->dongViOnDinh[4] = 0;
   
   // ---- Coban
   nguyenTo = &(mangNguyenTo[COBAN]);
   nguyenTo->so = 27;
   strcpy( nguyenTo->ten, "Coban" );
   strcpy( nguyenTo->kyHieu, "Co" );
   strcpy( nguyenTo->tenTrung, "钴" );
   nguyenTo->khoiLuong = 58.9332f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d⁷" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1768;
   nguyenTo->nhietDoSoi = 3200;
   nguyenTo->tiTrong = 8900;
   nguyenTo->nhietDung = 24.81f;
   nguyenTo->doAmDien = 1.88f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 760.4f;
   nguyenTo->nangLuongIonHoa2 = 1648;
   nguyenTo->banKinh = 125;  // ???
   nguyenTo->dongViOnDinh[0] = 58;
   nguyenTo->dongViOnDinh[1] = 0;
   
   // ---- Nicken
   nguyenTo = &(mangNguyenTo[NICKEN]);
   nguyenTo->so = 28;
   strcpy( nguyenTo->ten, "Nicken" );
   strcpy( nguyenTo->kyHieu, "Ni" );
   strcpy( nguyenTo->tenTrung, "镍" );
   nguyenTo->khoiLuong = 58.6934f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d⁸" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1728;
   nguyenTo->nhietDoSoi = 3186;
   nguyenTo->tiTrong = 8908;
   nguyenTo->nhietDung = 26.07f;
   nguyenTo->doAmDien = 1.91f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 737.1f;
   nguyenTo->nangLuongIonHoa2 = 1753;
   nguyenTo->banKinh = 124;  // ???
   nguyenTo->dongViOnDinh[0] = 58;
   nguyenTo->dongViOnDinh[1] = 60;
   nguyenTo->dongViOnDinh[2] = 61;
   nguyenTo->dongViOnDinh[3] = 62;
   nguyenTo->dongViOnDinh[4] = 64;
   nguyenTo->dongViOnDinh[5] = 0;
   
   // ---- Đồng
   nguyenTo = &(mangNguyenTo[DONG]);
   nguyenTo->so = 29;
   strcpy( nguyenTo->ten, "Đồng" );
   strcpy( nguyenTo->kyHieu, "Cu" );
   strcpy( nguyenTo->tenTrung, "铜" );
   nguyenTo->khoiLuong = 63.5463f;
   strcpy( nguyenTo->soOxyHoa, "+1,+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s¹3d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1357.77;
   nguyenTo->nhietDoSoi = 2835;
   nguyenTo->tiTrong = 8940;
   nguyenTo->nhietDung = 24.440f;
   nguyenTo->doAmDien = 1.90f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 745.5f;
   nguyenTo->nangLuongIonHoa2 = 1957.9;
   nguyenTo->banKinh = 140;
   nguyenTo->dongViOnDinh[0] = 63;
   nguyenTo->dongViOnDinh[1] = 65;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Kẽm
   nguyenTo = &(mangNguyenTo[KEM]);
   nguyenTo->so = 30;
   strcpy( nguyenTo->ten, "Kẽm" );
   strcpy( nguyenTo->kyHieu, "Zn" );
   strcpy( nguyenTo->tenTrung, "锌" );
   nguyenTo->khoiLuong = 65.382f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 692.68;
   nguyenTo->nhietDoSoi = 1180;
   nguyenTo->tiTrong = 7140;
   nguyenTo->nhietDung = 25.470f;
   nguyenTo->doAmDien = 1.65f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 906.4f;
   nguyenTo->nangLuongIonHoa2 = 1733.3;
   nguyenTo->banKinh = 139;
   nguyenTo->dongViOnDinh[0] = 64;
   nguyenTo->dongViOnDinh[1] = 66;
   nguyenTo->dongViOnDinh[2] = 67;
   nguyenTo->dongViOnDinh[3] = 68;
   nguyenTo->dongViOnDinh[4] = 70;
   nguyenTo->dongViOnDinh[5] = 0;
   
   // ---- Gali
   nguyenTo = &(mangNguyenTo[GALI]);
   nguyenTo->so = 31;
   strcpy( nguyenTo->ten, "Gali" );
   strcpy( nguyenTo->kyHieu, "Ga" );
   strcpy( nguyenTo->tenTrung, "镓" );
   nguyenTo->khoiLuong = 69.7231f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰4p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 302.9;
   nguyenTo->nhietDoSoi = 2477;
   nguyenTo->tiTrong = 5910;
   nguyenTo->nhietDung = 25.86f;
   nguyenTo->doAmDien = 1.81f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 578.8f;
   nguyenTo->nangLuongIonHoa2 = 1979.3;
   nguyenTo->banKinh = 187;
   nguyenTo->dongViOnDinh[0] = 69;
   nguyenTo->dongViOnDinh[1] = 71;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Gemani
   nguyenTo = &(mangNguyenTo[GEMANI]);
   nguyenTo->so = 32;
   strcpy( nguyenTo->ten, "Gemani" );
   strcpy( nguyenTo->kyHieu, "Ge" );
   strcpy( nguyenTo->tenTrung, "锗" );
   nguyenTo->khoiLuong = 72.631f;
   strcpy( nguyenTo->soOxyHoa, "+2,+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰4p²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1211.4;
   nguyenTo->nhietDoSoi = 3106;
   nguyenTo->tiTrong = 5323;
   nguyenTo->nhietDung = 23.222f;
   nguyenTo->doAmDien = 2.01f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_KIM_CUONG;
   nguyenTo->nangLuongIonHoa1 = 762;
   nguyenTo->nangLuongIonHoa2 = 1537.5;
   nguyenTo->banKinh = 211;
   nguyenTo->dongViOnDinh[0] = 70;
   nguyenTo->dongViOnDinh[1] = 72;
   nguyenTo->dongViOnDinh[2] = 73;
   nguyenTo->dongViOnDinh[3] = 74;
   nguyenTo->dongViOnDinh[4] = 76;
   nguyenTo->dongViOnDinh[5] = 0;

   // ---- Asen (Thạch Tin)
   nguyenTo = &(mangNguyenTo[ASEN]);
   nguyenTo->so = 33;
   strcpy( nguyenTo->ten, "Asen" );
   strcpy( nguyenTo->kyHieu, "As" );
   strcpy( nguyenTo->tenTrung, "砷" );
   nguyenTo->khoiLuong = 74.9216f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰4p³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 0.0f;
   nguyenTo->nhietDoSoi = 887;
   nguyenTo->tiTrong = 5727;
   nguyenTo->nhietDung = 24.64f;
   nguyenTo->doAmDien = 2.18f;
   nguyenTo->cauTrucTinhThe = BA_NGHIENG;
   nguyenTo->nangLuongIonHoa1 = 947;
   nguyenTo->nangLuongIonHoa2 = 1798;
   nguyenTo->banKinh = 185;
   nguyenTo->dongViOnDinh[0] = 75;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Seleni
   nguyenTo = &(mangNguyenTo[SELENI]);
   nguyenTo->so = 34;
   strcpy( nguyenTo->ten, "Seleni" );
   strcpy( nguyenTo->kyHieu, "Se" );
   strcpy( nguyenTo->tenTrung, "硒" );
   nguyenTo->khoiLuong = 78.96f;
   strcpy( nguyenTo->soOxyHoa, "+2,+4,+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰4p⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 494;
   nguyenTo->nhietDoSoi = 958;
   nguyenTo->tiTrong = 4280;
   nguyenTo->nhietDung = 25.363f;
   nguyenTo->doAmDien = 2.55f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 941;
   nguyenTo->nangLuongIonHoa2 = 2045;
   nguyenTo->banKinh = 190;
   nguyenTo->dongViOnDinh[0] = 74;
   nguyenTo->dongViOnDinh[1] = 76;
   nguyenTo->dongViOnDinh[2] = 77;
   nguyenTo->dongViOnDinh[3] = 78;
   nguyenTo->dongViOnDinh[4] = 80;
   nguyenTo->dongViOnDinh[5] = 82;
   nguyenTo->dongViOnDinh[6] = 0;

   // ---- Brom
   nguyenTo = &(mangNguyenTo[BROM]);
   nguyenTo->so = 35;
   strcpy( nguyenTo->ten, "Brom" );
   strcpy( nguyenTo->kyHieu, "Br" );
   strcpy( nguyenTo->tenTrung, "溴" );
   nguyenTo->khoiLuong = 79.9041f;
   strcpy( nguyenTo->soOxyHoa, "–1" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰4p⁵" );
   nguyenTo->loai = LONG;
   nguyenTo->nhietDoChay = 265.8f;
   nguyenTo->nhietDoSoi = 332;
   nguyenTo->tiTrong = 3102.8;
   nguyenTo->nhietDung = 25.69f; // <---- ????
   nguyenTo->doAmDien = 2.96f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 1139.9;
   nguyenTo->nangLuongIonHoa2 = 2103;
   nguyenTo->banKinh = 185;
   nguyenTo->dongViOnDinh[0] = 79;
   nguyenTo->dongViOnDinh[1] = 81;
   nguyenTo->dongViOnDinh[2] = 0;
   
   // ---- Krypton
   nguyenTo = &(mangNguyenTo[KRYPTON]);
   nguyenTo->so = 36;
   strcpy( nguyenTo->ten, "Krypton" );
   strcpy( nguyenTo->kyHieu, "Kr" );
   strcpy( nguyenTo->tenTrung, "氪" );
   nguyenTo->khoiLuong = 83.798f;
   strcpy( nguyenTo->soOxyHoa, "0" );
   strcpy( nguyenTo->cauHinhElecton, "[Ar] 4s²3d¹⁰4p⁶" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 115.79f;
   nguyenTo->nhietDoSoi = 119.93f;
   nguyenTo->tiTrong = 3.749;
   nguyenTo->nhietDung = 20.768f;
   nguyenTo->doAmDien = 0.0f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 1350.8;
   nguyenTo->nangLuongIonHoa2 = 2350.4;
   nguyenTo->banKinh = 202;
   nguyenTo->dongViOnDinh[0] = 78;
   nguyenTo->dongViOnDinh[1] = 80;
   nguyenTo->dongViOnDinh[2] = 82;
   nguyenTo->dongViOnDinh[3] = 83;
   nguyenTo->dongViOnDinh[4] = 84;
   nguyenTo->dongViOnDinh[5] = 86;
   nguyenTo->dongViOnDinh[6] = 0;

   // ---- Rubiđi
   nguyenTo = &(mangNguyenTo[RUBIDI]);
   nguyenTo->so = 37;
   strcpy( nguyenTo->ten, "Rubiđi" );
   strcpy( nguyenTo->kyHieu, "Rb" );
   strcpy( nguyenTo->tenTrung, "铷" );
   nguyenTo->khoiLuong = 85.4678f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 312.46f;
   nguyenTo->nhietDoSoi = 961;
   nguyenTo->tiTrong = 1532;
   nguyenTo->nhietDung = 31.060f;
   nguyenTo->doAmDien = 0.82f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 403;
   nguyenTo->nangLuongIonHoa2 = 2632.1f;
   nguyenTo->banKinh = 303;
   nguyenTo->dongViOnDinh[0] = 85;
   nguyenTo->dongViOnDinh[1] = 87;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Stronti
   nguyenTo = &(mangNguyenTo[STRONTI]);
   nguyenTo->so = 38;
   strcpy( nguyenTo->ten, "Stronti" );
   strcpy( nguyenTo->kyHieu, "Sr" );
   strcpy( nguyenTo->tenTrung, "锶" );
   nguyenTo->khoiLuong = 87.62f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1050;
   nguyenTo->nhietDoSoi = 1655;
   nguyenTo->tiTrong = 2640;
   nguyenTo->nhietDung = 26.4;
   nguyenTo->doAmDien = 0.95f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 549.5;
   nguyenTo->nangLuongIonHoa2 = 1064.2f;
   nguyenTo->banKinh = 249;
   nguyenTo->dongViOnDinh[0] = 84;
   nguyenTo->dongViOnDinh[1] = 86;
   nguyenTo->dongViOnDinh[2] = 87;
   nguyenTo->dongViOnDinh[3] = 88;
   nguyenTo->dongViOnDinh[4] = 0;

   // ---- Ytri
   nguyenTo = &(mangNguyenTo[YTRI]);
   nguyenTo->so = 39;
   strcpy( nguyenTo->ten, "Ytri" );
   strcpy( nguyenTo->kyHieu, "Y" );
   strcpy( nguyenTo->tenTrung, "钇" );
   nguyenTo->khoiLuong = 88.9059f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1799;
   nguyenTo->nhietDoSoi = 3609;
   nguyenTo->tiTrong = 4472;
   nguyenTo->nhietDung = 26.53;
   nguyenTo->doAmDien = 1.22f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 600;
   nguyenTo->nangLuongIonHoa2 = 1180;
   nguyenTo->banKinh = 190; // <---- ???
   nguyenTo->dongViOnDinh[0] = 89;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Zirconi
   nguyenTo = &(mangNguyenTo[ZIRCONI]);
   nguyenTo->so = 40;
   strcpy( nguyenTo->ten, "Zirconi" );
   strcpy( nguyenTo->kyHieu, "Zr" );
   strcpy( nguyenTo->tenTrung, "锆" );
   nguyenTo->khoiLuong = 91.224f;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2128;
   nguyenTo->nhietDoSoi = 4682;
   nguyenTo->tiTrong = 6520;
   nguyenTo->nhietDung = 25.36f;
   nguyenTo->doAmDien = 1.233;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;  // ???
   nguyenTo->nangLuongIonHoa1 = 640.1f;
   nguyenTo->nangLuongIonHoa2 = 1270;
   nguyenTo->banKinh = 175; // <---- ???
   nguyenTo->dongViOnDinh[0] = 90;
   nguyenTo->dongViOnDinh[1] = 91;
   nguyenTo->dongViOnDinh[2] = 92;
   nguyenTo->dongViOnDinh[3] = 94;
   nguyenTo->dongViOnDinh[4] = 96;
   nguyenTo->dongViOnDinh[5] = 0;

   // ---- Niobi
   nguyenTo = &(mangNguyenTo[NIOBI]);
   nguyenTo->so = 41;
   strcpy( nguyenTo->ten, "Niobi" );
   strcpy( nguyenTo->kyHieu, "Nb" );
   strcpy( nguyenTo->tenTrung, "铌" );
   nguyenTo->khoiLuong = 92.9064f;
   strcpy( nguyenTo->soOxyHoa, "+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s¹4d⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2750;
   nguyenTo->nhietDoSoi = 5017;
   nguyenTo->tiTrong = 8570;
   nguyenTo->nhietDung = 24.60f;
   nguyenTo->doAmDien = 1.60;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 652.1f;
   nguyenTo->nangLuongIonHoa2 = 1380;
   nguyenTo->banKinh = 164; // <---- ???
   nguyenTo->dongViOnDinh[0] = 93;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Molipđen
   nguyenTo = &(mangNguyenTo[MOLIPDEN]);
   nguyenTo->so = 42;
   strcpy( nguyenTo->ten, "Molipđen" );
   strcpy( nguyenTo->kyHieu, "Mo" );
   strcpy( nguyenTo->tenTrung, "钼" );
   nguyenTo->khoiLuong = 95.941f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+4,+5,+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s¹4d⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2896;
   nguyenTo->nhietDoSoi = 4912;
   nguyenTo->tiTrong = 8570;
   nguyenTo->nhietDung = 24.06f;
   nguyenTo->doAmDien = 2.16;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 684.3f;
   nguyenTo->nangLuongIonHoa2 = 1560;
   nguyenTo->banKinh = 154; // <---- ???
   nguyenTo->dongViOnDinh[0] = 92;
   nguyenTo->dongViOnDinh[1] = 94;
   nguyenTo->dongViOnDinh[2] = 95;
   nguyenTo->dongViOnDinh[3] = 96;
   nguyenTo->dongViOnDinh[4] = 97;
   nguyenTo->dongViOnDinh[5] = 98;
   nguyenTo->dongViOnDinh[6] = 100;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Tecnexi
   nguyenTo = &(mangNguyenTo[TECNEXI]);
   nguyenTo->so = 43;
   strcpy( nguyenTo->ten, "Tecnexi" );
   strcpy( nguyenTo->kyHieu, "Tc" );
   strcpy( nguyenTo->tenTrung, "锝" );
   nguyenTo->khoiLuong = 98.0f;
   strcpy( nguyenTo->soOxyHoa, "+4,+7" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2430;
   nguyenTo->nhietDoSoi = 4538;
   nguyenTo->tiTrong = 11000;
   nguyenTo->nhietDung = 24.27f;
   nguyenTo->doAmDien = 1.9f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 702;
   nguyenTo->nangLuongIonHoa2 = 1470;
   nguyenTo->banKinh = 147; // <---- ???
   nguyenTo->dongViOnDinh[0] = 0;
   
   // ---- Ruteni
   nguyenTo = &(mangNguyenTo[RUTENI]);
   nguyenTo->so = 44;
   strcpy( nguyenTo->ten, "Ruteni" );
   strcpy( nguyenTo->kyHieu, "Ru" );
   strcpy( nguyenTo->tenTrung, "钌" );
   nguyenTo->khoiLuong = 101.07f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s¹4d⁷" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2607;
   nguyenTo->nhietDoSoi = 4423;
   nguyenTo->tiTrong = 12450;
   nguyenTo->nhietDung = 24.06f;
   nguyenTo->doAmDien = 2.3f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 710.2;
   nguyenTo->nangLuongIonHoa2 = 1620;
   nguyenTo->banKinh = 146; // <---- ???
   nguyenTo->dongViOnDinh[0] = 96;
   nguyenTo->dongViOnDinh[1] = 98;
   nguyenTo->dongViOnDinh[2] = 99;
   nguyenTo->dongViOnDinh[3] = 100;
   nguyenTo->dongViOnDinh[4] = 101;
   nguyenTo->dongViOnDinh[5] = 102;
   nguyenTo->dongViOnDinh[6] = 104;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Rôđi
   nguyenTo = &(mangNguyenTo[RODI]);
   nguyenTo->so = 45;
   strcpy( nguyenTo->ten, "Rôđi" );
   strcpy( nguyenTo->kyHieu, "Rh" );
   strcpy( nguyenTo->tenTrung, "铑" );
   nguyenTo->khoiLuong = 102.9055f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s¹4d⁸" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2237;
   nguyenTo->nhietDoSoi = 3968;
   nguyenTo->tiTrong = 12410;
   nguyenTo->nhietDung = 24.98f;
   nguyenTo->doAmDien = 2.28f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 719.7;
   nguyenTo->nangLuongIonHoa2 = 1740;
   nguyenTo->banKinh = 142; // <---- ???
   nguyenTo->dongViOnDinh[0] = 103;
   nguyenTo->dongViOnDinh[1] = 0;
   
   // ---- Paladi
   nguyenTo = &(mangNguyenTo[PALADI]);
   nguyenTo->so = 46;
   strcpy( nguyenTo->ten, "Paladi" );
   strcpy( nguyenTo->kyHieu, "Pd" );
   strcpy( nguyenTo->tenTrung, "钯" );
   nguyenTo->khoiLuong = 106.42f;
   strcpy( nguyenTo->soOxyHoa, "+2,+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 4d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1828.05f;
   nguyenTo->nhietDoSoi = 3236;
   nguyenTo->tiTrong = 12023;
   nguyenTo->nhietDung = 25.98f;
   nguyenTo->doAmDien = 2.20f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 804.4f;
   nguyenTo->nangLuongIonHoa2 = 1870;
   nguyenTo->banKinh = 163;
   nguyenTo->dongViOnDinh[0] = 102;
   nguyenTo->dongViOnDinh[1] = 104;
   nguyenTo->dongViOnDinh[2] = 105;
   nguyenTo->dongViOnDinh[3] = 106;
   nguyenTo->dongViOnDinh[4] = 108;
   nguyenTo->dongViOnDinh[5] = 110;
   nguyenTo->dongViOnDinh[6] = 0;

   // ---- Bạc
   nguyenTo = &(mangNguyenTo[BAC]);
   nguyenTo->so = 47;
   strcpy( nguyenTo->ten, "Bạc" );
   strcpy( nguyenTo->kyHieu, "Ag" );
   strcpy( nguyenTo->tenTrung, "銀" );
   nguyenTo->khoiLuong = 107.8682f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s¹4d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1234.93f;
   nguyenTo->nhietDoSoi = 2435;
   nguyenTo->tiTrong = 10490;
   nguyenTo->nhietDung = 25.350f;
   nguyenTo->doAmDien = 1.93f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 731.0f;
   nguyenTo->nangLuongIonHoa2 = 2070;
   nguyenTo->banKinh = 172;
   nguyenTo->dongViOnDinh[0] = 107;
   nguyenTo->dongViOnDinh[1] = 109;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Cađimi
   nguyenTo = &(mangNguyenTo[CADIMI]);
   nguyenTo->so = 48;
   strcpy( nguyenTo->ten, "Cađimi" );
   strcpy( nguyenTo->kyHieu, "Cd" );
   strcpy( nguyenTo->tenTrung, "镉" );
   nguyenTo->khoiLuong = 112.411f;
   strcpy( nguyenTo->soOxyHoa, "+1, +2" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 594.22f;
   nguyenTo->nhietDoSoi = 1040;
   nguyenTo->tiTrong = 8650;
   nguyenTo->nhietDung = 26.020f;
   nguyenTo->doAmDien = 1.69f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 867.8f;
   nguyenTo->nangLuongIonHoa2 = 1631.4f;
   nguyenTo->banKinh = 172;
   nguyenTo->dongViOnDinh[0] = 106;
   nguyenTo->dongViOnDinh[1] = 108;
   nguyenTo->dongViOnDinh[2] = 110;
   nguyenTo->dongViOnDinh[3] = 111;
   nguyenTo->dongViOnDinh[4] = 112;
   nguyenTo->dongViOnDinh[5] = 113;
   nguyenTo->dongViOnDinh[6] = 114;
   nguyenTo->dongViOnDinh[7] = 116;
   nguyenTo->dongViOnDinh[8] = 0;

   // ---- Inđi
   nguyenTo = &(mangNguyenTo[INDI]);
   nguyenTo->so = 49;
   strcpy( nguyenTo->ten, "Inđi" );
   strcpy( nguyenTo->kyHieu, "In" );
   strcpy( nguyenTo->tenTrung, "铟" );
   nguyenTo->khoiLuong = 114.818f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰5p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 429.749f;
   nguyenTo->nhietDoSoi = 2345;
   nguyenTo->tiTrong = 7310;
   nguyenTo->nhietDung = 26.74f;
   nguyenTo->doAmDien = 1.78f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 558.3f;
   nguyenTo->nangLuongIonHoa2 = 1820.7f;
   nguyenTo->banKinh = 193;
   nguyenTo->dongViOnDinh[0] = 113;
   nguyenTo->dongViOnDinh[1] = 115;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Thiếc
   nguyenTo = &(mangNguyenTo[THIEC]);
   nguyenTo->so = 50;
   strcpy( nguyenTo->ten, "Thiếc" );
   strcpy( nguyenTo->kyHieu, "Sn" );
   strcpy( nguyenTo->tenTrung, "锡" );
   nguyenTo->khoiLuong = 118.710f;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰5p²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 429.749f;
   nguyenTo->nhietDoSoi = 2345;
   nguyenTo->tiTrong = 7310;
   nguyenTo->nhietDung = 26.74f;
   nguyenTo->doAmDien = 1.96f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 708.6f;
   nguyenTo->nangLuongIonHoa2 = 1411.8f;
   nguyenTo->banKinh = 217;
   nguyenTo->dongViOnDinh[0] = 112;
   nguyenTo->dongViOnDinh[1] = 114;
   nguyenTo->dongViOnDinh[2] = 115;
   nguyenTo->dongViOnDinh[3] = 116;
   nguyenTo->dongViOnDinh[4] = 117;
   nguyenTo->dongViOnDinh[5] = 118;
   nguyenTo->dongViOnDinh[6] = 119;
   nguyenTo->dongViOnDinh[7] = 120;
   nguyenTo->dongViOnDinh[8] = 122;
   nguyenTo->dongViOnDinh[9] = 124;
   nguyenTo->dongViOnDinh[10] = 0;

   // ---- Antimon
   nguyenTo = &(mangNguyenTo[ANTIMON]);
   nguyenTo->so = 51;
   strcpy( nguyenTo->ten, "Antimon" );
   strcpy( nguyenTo->kyHieu, "Sb" );
   strcpy( nguyenTo->tenTrung, "锑" );
   nguyenTo->khoiLuong = 121.7601f;
   strcpy( nguyenTo->soOxyHoa, "+3,+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰5p³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 903.78f;
   nguyenTo->nhietDoSoi = 1860;
   nguyenTo->tiTrong = 6697;
   nguyenTo->nhietDung = 25.23f;
   nguyenTo->doAmDien = 2.05f;
   nguyenTo->cauTrucTinhThe = BA_NGHIENG;
   nguyenTo->nangLuongIonHoa1 = 834;
   nguyenTo->nangLuongIonHoa2 = 1594.9f;
   nguyenTo->banKinh = 206;
   nguyenTo->dongViOnDinh[0] = 121;
   nguyenTo->dongViOnDinh[1] = 123;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Telu
   nguyenTo = &(mangNguyenTo[TELU]);
   nguyenTo->so = 52;
   strcpy( nguyenTo->ten, "Telu" );
   strcpy( nguyenTo->kyHieu, "Te" );
   strcpy( nguyenTo->tenTrung, "碲" );
   nguyenTo->khoiLuong = 127.60;
   strcpy( nguyenTo->soOxyHoa, "+4,+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰5p⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 722.66f;
   nguyenTo->nhietDoSoi = 1261;
   nguyenTo->tiTrong = 6697;
   nguyenTo->nhietDung = 25.73f;
   nguyenTo->doAmDien = 2.1f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 869.3f;
   nguyenTo->nangLuongIonHoa2 = 1790;
   nguyenTo->banKinh = 206;
   nguyenTo->dongViOnDinh[0] = 120;
   nguyenTo->dongViOnDinh[1] = 122;
   nguyenTo->dongViOnDinh[2] = 123;
   nguyenTo->dongViOnDinh[3] = 124;
   nguyenTo->dongViOnDinh[4] = 125;
   nguyenTo->dongViOnDinh[5] = 126;
   nguyenTo->dongViOnDinh[6] = 128;
   nguyenTo->dongViOnDinh[7] = 130;
   nguyenTo->dongViOnDinh[8] = 0;

   // ---- Iot
   nguyenTo = &(mangNguyenTo[IOT]);
   nguyenTo->so = 53;
   strcpy( nguyenTo->ten, "Iot" );
   strcpy( nguyenTo->kyHieu, "I" );
   strcpy( nguyenTo->tenTrung, "碘" );
   nguyenTo->khoiLuong = 126.9045;
   strcpy( nguyenTo->soOxyHoa, "–1" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰5p⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 386.85f;
   nguyenTo->nhietDoSoi = 457.4;
   nguyenTo->tiTrong = 4933;
   nguyenTo->nhietDung = 54.44f;
   nguyenTo->doAmDien = 2.66f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 1008.4f;
   nguyenTo->nangLuongIonHoa2 = 1845.9f;
   nguyenTo->banKinh = 198;
   nguyenTo->dongViOnDinh[0] = 127;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Xenon
   nguyenTo = &(mangNguyenTo[XENON]);
   nguyenTo->so = 54;
   strcpy( nguyenTo->ten, "Xenon" );
   strcpy( nguyenTo->kyHieu, "Xe" );
   strcpy( nguyenTo->tenTrung, "氙" );
   nguyenTo->khoiLuong = 131.2936f;
   strcpy( nguyenTo->soOxyHoa, "0" );
   strcpy( nguyenTo->cauHinhElecton, "[Kr] 5s²4d¹⁰5p⁶" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 161.4f;
   nguyenTo->nhietDoSoi = 165.03;
   nguyenTo->tiTrong = 5.894;
   nguyenTo->nhietDung = 20.786;
   nguyenTo->doAmDien = 2.6f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 1170.4f;
   nguyenTo->nangLuongIonHoa2 = 2046.4f;
   nguyenTo->banKinh = 216;
   nguyenTo->dongViOnDinh[0] = 124;
   nguyenTo->dongViOnDinh[1] = 126;
   nguyenTo->dongViOnDinh[2] = 128;
   nguyenTo->dongViOnDinh[3] = 129;
   nguyenTo->dongViOnDinh[4] = 130;
   nguyenTo->dongViOnDinh[5] = 131;
   nguyenTo->dongViOnDinh[6] = 132;
   nguyenTo->dongViOnDinh[7] = 134;
   nguyenTo->dongViOnDinh[8] = 136;
   nguyenTo->dongViOnDinh[9] = 0;

   // ---- Xesi
   nguyenTo = &(mangNguyenTo[XESI]);
   nguyenTo->so = 55;
   strcpy( nguyenTo->ten, "Xesi" );
   strcpy( nguyenTo->kyHieu, "Cs" );
   strcpy( nguyenTo->tenTrung, "铯" );
   nguyenTo->khoiLuong = 132.9055f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 301.59f;
   nguyenTo->nhietDoSoi = 944;
   nguyenTo->tiTrong = 1930;
   nguyenTo->nhietDung = 32.210;
   nguyenTo->doAmDien = 0.79f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 375.7f;
   nguyenTo->nangLuongIonHoa2 = 2234.3f;
   nguyenTo->banKinh = 343;
   nguyenTo->dongViOnDinh[0] = 133;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Bari
   nguyenTo = &(mangNguyenTo[BARI]);
   nguyenTo->so = 56;
   strcpy( nguyenTo->ten, "Bari" );
   strcpy( nguyenTo->kyHieu, "Ba" );
   strcpy( nguyenTo->tenTrung, "钡" );
   nguyenTo->khoiLuong = 137.3277f;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1000;
   nguyenTo->nhietDoSoi = 2118;
   nguyenTo->tiTrong = 3510;
   nguyenTo->nhietDung = 28.07;
   nguyenTo->doAmDien = 0.89f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 502.9f;
   nguyenTo->nangLuongIonHoa2 = 965.2f;
   nguyenTo->banKinh = 268;
   nguyenTo->dongViOnDinh[0] = 130;
   nguyenTo->dongViOnDinh[1] = 132;
   nguyenTo->dongViOnDinh[2] = 134;
   nguyenTo->dongViOnDinh[3] = 135;
   nguyenTo->dongViOnDinh[4] = 136;
   nguyenTo->dongViOnDinh[5] = 137;
   nguyenTo->dongViOnDinh[6] = 138;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Lantan
   nguyenTo = &(mangNguyenTo[LANTAN]);
   nguyenTo->so = 57;
   strcpy( nguyenTo->ten, "Lantan" );
   strcpy( nguyenTo->kyHieu, "La" );
   strcpy( nguyenTo->tenTrung, "镧" );
   nguyenTo->khoiLuong = 138.9055f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²5d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1193;
   nguyenTo->nhietDoSoi = 3737;
   nguyenTo->tiTrong = 6162;
   nguyenTo->nhietDung = 27.11;
   nguyenTo->doAmDien = 1.10f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 538.1f;
   nguyenTo->nangLuongIonHoa2 = 1067;
   nguyenTo->banKinh = 207;
   nguyenTo->dongViOnDinh[0] = 138;
   nguyenTo->dongViOnDinh[1] = 139;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Xeri
   nguyenTo = &(mangNguyenTo[XERI]);
   nguyenTo->so = 58;
   strcpy( nguyenTo->ten, "Xeri" );
   strcpy( nguyenTo->kyHieu, "Ce" );
   strcpy( nguyenTo->tenTrung, "铈" );
   nguyenTo->khoiLuong = 140.116f;
   strcpy( nguyenTo->soOxyHoa, "+3,+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²5d¹4f¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1068;
   nguyenTo->nhietDoSoi = 3716;
   nguyenTo->tiTrong = 6770;
   nguyenTo->nhietDung = 26.94;
   nguyenTo->doAmDien = 1.12f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 534.4f;
   nguyenTo->nangLuongIonHoa2 = 1050;
   nguyenTo->banKinh = 204;
   nguyenTo->dongViOnDinh[0] = 136;
   nguyenTo->dongViOnDinh[1] = 138;
   nguyenTo->dongViOnDinh[2] = 140;
   nguyenTo->dongViOnDinh[3] = 142;
   nguyenTo->dongViOnDinh[4] = 0;

   // ---- Prazeođim
   nguyenTo = &(mangNguyenTo[PRAZEODIM]);
   nguyenTo->so = 59;
   strcpy( nguyenTo->ten, "Prazeođim" );
   strcpy( nguyenTo->kyHieu, "Pr" );
   strcpy( nguyenTo->tenTrung, "镨" );
   nguyenTo->khoiLuong = 140.9077f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1208;
   nguyenTo->nhietDoSoi = 3793;
   nguyenTo->tiTrong = 6770;
   nguyenTo->nhietDung = 27.20;
   nguyenTo->doAmDien = 1.13f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 527;
   nguyenTo->nangLuongIonHoa2 = 1020;
   nguyenTo->banKinh = 203;
   nguyenTo->dongViOnDinh[0] = 141;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Neođim
   nguyenTo = &(mangNguyenTo[NEODYMI]);
   nguyenTo->so = 60;
   strcpy( nguyenTo->ten, "Neođim" );
   strcpy( nguyenTo->kyHieu, "Nd" );
   strcpy( nguyenTo->tenTrung, "钕" );
   nguyenTo->khoiLuong = 144.242f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1297;
   nguyenTo->nhietDoSoi = 3347;
   nguyenTo->tiTrong = 7010;
   nguyenTo->nhietDung = 27.45;
   nguyenTo->doAmDien = 1.14f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 533.1f;
   nguyenTo->nangLuongIonHoa2 = 1040;
   nguyenTo->banKinh = 201;
   nguyenTo->dongViOnDinh[0] = 142;
   nguyenTo->dongViOnDinh[1] = 143;
   nguyenTo->dongViOnDinh[2] = 144;
   nguyenTo->dongViOnDinh[3] = 145;
   nguyenTo->dongViOnDinh[4] = 146;
   nguyenTo->dongViOnDinh[5] = 148;
   nguyenTo->dongViOnDinh[6] = 150;
   nguyenTo->dongViOnDinh[7] = 0;
   
   // ---- Prometi
   nguyenTo = &(mangNguyenTo[PROMETI]);
   nguyenTo->so = 61;
   strcpy( nguyenTo->ten, "Prometi" );
   strcpy( nguyenTo->kyHieu, "Pm" );
   strcpy( nguyenTo->tenTrung, "钷" );
   nguyenTo->khoiLuong = 145;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1315;
   nguyenTo->nhietDoSoi = 3273;
   nguyenTo->tiTrong = 7260;
   nguyenTo->nhietDung = 0.0f;
   nguyenTo->doAmDien = 1.13f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 540;
   nguyenTo->nangLuongIonHoa2 = 1050;
   nguyenTo->banKinh = 199;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Samari
   nguyenTo = &(mangNguyenTo[SAMARI]);
   nguyenTo->so = 62;
   strcpy( nguyenTo->ten, "Samari" );
   strcpy( nguyenTo->kyHieu, "Sm" );
   strcpy( nguyenTo->tenTrung, "钐" );
   nguyenTo->khoiLuong = 150.36f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f⁶" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1345;
   nguyenTo->nhietDoSoi = 2067;
   nguyenTo->tiTrong = 7520;
   nguyenTo->nhietDung = 29.54f;
   nguyenTo->doAmDien = 1.17f;
   nguyenTo->cauTrucTinhThe = BA_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 544.5f;
   nguyenTo->nangLuongIonHoa2 = 1070;
   nguyenTo->banKinh = 198;
   nguyenTo->dongViOnDinh[0] = 144;
   nguyenTo->dongViOnDinh[1] = 147;
   nguyenTo->dongViOnDinh[2] = 148;
   nguyenTo->dongViOnDinh[3] = 149;
   nguyenTo->dongViOnDinh[4] = 150;
   nguyenTo->dongViOnDinh[5] = 152;
   nguyenTo->dongViOnDinh[6] = 154;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Europi
   nguyenTo = &(mangNguyenTo[EUROPI]);
   nguyenTo->so = 63;
   strcpy( nguyenTo->ten, "Europi" );
   strcpy( nguyenTo->kyHieu, "Eu" );
   strcpy( nguyenTo->tenTrung, "铕" );
   nguyenTo->khoiLuong = 151.964f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f⁷" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1099;
   nguyenTo->nhietDoSoi = 1802;
   nguyenTo->tiTrong = 5264;
   nguyenTo->nhietDung = 27.66f;
   nguyenTo->doAmDien = 1.2f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 547.1f;
   nguyenTo->nangLuongIonHoa2 = 1085;
   nguyenTo->banKinh = 198;
   nguyenTo->dongViOnDinh[0] = 151;
   nguyenTo->dongViOnDinh[1] = 153;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Gađolini
   nguyenTo = &(mangNguyenTo[GADOLINI]);
   nguyenTo->so = 64;
   strcpy( nguyenTo->ten, "Gađolini" );
   strcpy( nguyenTo->kyHieu, "Gd" );
   strcpy( nguyenTo->tenTrung, "钆" );
   nguyenTo->khoiLuong = 157.25f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f⁷5d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1585;
   nguyenTo->nhietDoSoi = 3546;
   nguyenTo->tiTrong = 7900;
   nguyenTo->nhietDung = 37.03f;
   nguyenTo->doAmDien = 1.2f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 593.4f;
   nguyenTo->nangLuongIonHoa2 = 1170;
   nguyenTo->banKinh = 196;
   nguyenTo->dongViOnDinh[0] = 152;
   nguyenTo->dongViOnDinh[1] = 154;
   nguyenTo->dongViOnDinh[2] = 155;
   nguyenTo->dongViOnDinh[3] = 156;
   nguyenTo->dongViOnDinh[4] = 157;
   nguyenTo->dongViOnDinh[5] = 158;
   nguyenTo->dongViOnDinh[6] = 160;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Tebi
   nguyenTo = &(mangNguyenTo[TEBI]);
   nguyenTo->so = 65;
   strcpy( nguyenTo->ten, "Tebi" );
   strcpy( nguyenTo->kyHieu, "Tb" );
   strcpy( nguyenTo->tenTrung, "铽" );
   nguyenTo->khoiLuong = 158.9254f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f⁹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1629;
   nguyenTo->nhietDoSoi = 3503;
   nguyenTo->tiTrong = 8230;
   nguyenTo->nhietDung = 28.91f;
   nguyenTo->doAmDien = 1.2f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 565.8f;
   nguyenTo->nangLuongIonHoa2 = 1110;
   nguyenTo->banKinh = 194;
   nguyenTo->dongViOnDinh[0] = 159;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Điprozi
   nguyenTo = &(mangNguyenTo[DIPROZI]);
   nguyenTo->so = 66;
   strcpy( nguyenTo->ten, "Điprozi" );
   strcpy( nguyenTo->kyHieu, "Dy" );
   strcpy( nguyenTo->tenTrung, "镝" );
   nguyenTo->khoiLuong = 162.500f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1680;
   nguyenTo->nhietDoSoi = 2840;
   nguyenTo->tiTrong = 8540;
   nguyenTo->nhietDung = 27.70f;
   nguyenTo->doAmDien = 1.22f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 573.0f;
   nguyenTo->nangLuongIonHoa2 = 1130;
   nguyenTo->banKinh = 192;
   nguyenTo->dongViOnDinh[0] = 156;
   nguyenTo->dongViOnDinh[1] = 158;
   nguyenTo->dongViOnDinh[2] = 160;
   nguyenTo->dongViOnDinh[3] = 161;
   nguyenTo->dongViOnDinh[4] = 162;
   nguyenTo->dongViOnDinh[5] = 163;
   nguyenTo->dongViOnDinh[6] = 164;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Honmi
   nguyenTo = &(mangNguyenTo[HONMI]);
   nguyenTo->so = 67;
   strcpy( nguyenTo->ten, "Honmi" );
   strcpy( nguyenTo->kyHieu, "Ho" );
   strcpy( nguyenTo->tenTrung, "钬" );
   nguyenTo->khoiLuong = 164.9303f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1747;
   nguyenTo->nhietDoSoi = 2973;
   nguyenTo->tiTrong = 8790;
   nguyenTo->nhietDung = 27.15f;
   nguyenTo->doAmDien = 1.22f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 581.0f;
   nguyenTo->nangLuongIonHoa2 = 1140;
   nguyenTo->banKinh = 192;
   nguyenTo->dongViOnDinh[0] = 165;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Eribi
   nguyenTo = &(mangNguyenTo[ERBI]);
   nguyenTo->so = 68;
   strcpy( nguyenTo->ten, "Eribi" );
   strcpy( nguyenTo->kyHieu, "Er" );
   strcpy( nguyenTo->tenTrung, "铒" );
   nguyenTo->khoiLuong = 167.259f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1802;
   nguyenTo->nhietDoSoi = 3141;
   nguyenTo->tiTrong = 9066;
   nguyenTo->nhietDung = 28.12f;
   nguyenTo->doAmDien = 1.24f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 589.3f;
   nguyenTo->nangLuongIonHoa2 = 1150;
   nguyenTo->banKinh = 189;
   nguyenTo->dongViOnDinh[0] = 162;
   nguyenTo->dongViOnDinh[1] = 164;
   nguyenTo->dongViOnDinh[2] = 166;
   nguyenTo->dongViOnDinh[3] = 167;
   nguyenTo->dongViOnDinh[4] = 168;
   nguyenTo->dongViOnDinh[5] = 170;
   nguyenTo->dongViOnDinh[6] = 0;

   // ---- Tuli
   nguyenTo = &(mangNguyenTo[TULI]);
   nguyenTo->so = 69;
   strcpy( nguyenTo->ten, "Tuli" );
   strcpy( nguyenTo->kyHieu, "Tm" );
   strcpy( nguyenTo->tenTrung, "铥" );
   nguyenTo->khoiLuong = 168.9342f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1818;
   nguyenTo->nhietDoSoi = 2223;
   nguyenTo->tiTrong = 9320;
   nguyenTo->nhietDung = 27.03f;
   nguyenTo->doAmDien = 1.24f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 596.7f;
   nguyenTo->nangLuongIonHoa2 = 1160;
   nguyenTo->banKinh = 190;
   nguyenTo->dongViOnDinh[0] = 169;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Ytecbi
   nguyenTo = &(mangNguyenTo[YTECBI]);
   nguyenTo->so = 70;
   strcpy( nguyenTo->ten, "Ytecbi" );
   strcpy( nguyenTo->kyHieu, "Yb" );
   strcpy( nguyenTo->tenTrung, "镱" );
   nguyenTo->khoiLuong = 168.9342f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1097;
   nguyenTo->nhietDoSoi = 1469;
   nguyenTo->tiTrong = 6900;
   nguyenTo->nhietDung = 26.74f;
   nguyenTo->doAmDien = 1.1f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 603.4f;
   nguyenTo->nangLuongIonHoa2 = 1174.8;
   nguyenTo->banKinh = 187;
   nguyenTo->dongViOnDinh[0] = 168;
   nguyenTo->dongViOnDinh[1] = 170;
   nguyenTo->dongViOnDinh[2] = 171;
   nguyenTo->dongViOnDinh[3] = 172;
   nguyenTo->dongViOnDinh[4] = 173;
   nguyenTo->dongViOnDinh[5] = 174;
   nguyenTo->dongViOnDinh[6] = 176;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Lutexi
   nguyenTo = &(mangNguyenTo[LUTEXI]);
   nguyenTo->so = 71;
   strcpy( nguyenTo->ten, "Lutexi" );
   strcpy( nguyenTo->kyHieu, "Lu" );
   strcpy( nguyenTo->tenTrung, "镥" );
   nguyenTo->khoiLuong = 174.9668f;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1925;
   nguyenTo->nhietDoSoi = 3675;
   nguyenTo->tiTrong = 9841;
   nguyenTo->nhietDung = 26.86f;
   nguyenTo->doAmDien = 1.27f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 523.5f;
   nguyenTo->nangLuongIonHoa2 = 1340;
   nguyenTo->banKinh = 174;    // <------
   nguyenTo->dongViOnDinh[0] = 175;
   nguyenTo->dongViOnDinh[1] = 176;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Hafini
   nguyenTo = &(mangNguyenTo[HAFINI]);
   nguyenTo->so = 72;
   strcpy( nguyenTo->ten, "Hafini" );
   strcpy( nguyenTo->kyHieu, "Hf" );
   strcpy( nguyenTo->tenTrung, "铪" );
   nguyenTo->khoiLuong = 178.49f;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2506;
   nguyenTo->nhietDoSoi = 4876;
   nguyenTo->tiTrong = 13310;
   nguyenTo->nhietDung = 25.73f;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 658.5f;
   nguyenTo->nangLuongIonHoa2 = 1440;
   nguyenTo->banKinh = 175;    // <------
   nguyenTo->dongViOnDinh[0] = 174;
   nguyenTo->dongViOnDinh[1] = 176;
   nguyenTo->dongViOnDinh[2] = 177;
   nguyenTo->dongViOnDinh[3] = 178;
   nguyenTo->dongViOnDinh[4] = 179;
   nguyenTo->dongViOnDinh[5] = 180;
   nguyenTo->dongViOnDinh[6] = 0;

   // ---- Tantan
   nguyenTo = &(mangNguyenTo[TANTAN]);
   nguyenTo->so = 73;
   strcpy( nguyenTo->ten, "Tantan" );
   strcpy( nguyenTo->kyHieu, "Ta" );
   strcpy( nguyenTo->tenTrung, "钽" );
   nguyenTo->khoiLuong = 180.9479f;
   strcpy( nguyenTo->soOxyHoa, "+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 3290;
   nguyenTo->nhietDoSoi = 5731;
   nguyenTo->tiTrong = 16690;
   nguyenTo->nhietDung = 25.36f;
   nguyenTo->doAmDien = 1.5f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 761;
   nguyenTo->nangLuongIonHoa2 = 1500;
   nguyenTo->banKinh = 170;    // <------
   nguyenTo->dongViOnDinh[0] = 181;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Vonfam
   nguyenTo = &(mangNguyenTo[VONFAM]);
   nguyenTo->so = 74;
   strcpy( nguyenTo->ten, "Vonfam" );
   strcpy( nguyenTo->kyHieu, "W" );
   strcpy( nguyenTo->tenTrung, "鎢" );
   nguyenTo->khoiLuong = 183.84f;
   strcpy( nguyenTo->soOxyHoa, "+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 3695;
   nguyenTo->nhietDoSoi = 5828;
   nguyenTo->tiTrong = 19250;
   nguyenTo->nhietDung = 24.27f;
   nguyenTo->doAmDien = 2.36f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 770;
   nguyenTo->nangLuongIonHoa2 = 1700;
   nguyenTo->banKinh = 162;    // <------
   nguyenTo->dongViOnDinh[0] = 180;
   nguyenTo->dongViOnDinh[1] = 182;
   nguyenTo->dongViOnDinh[2] = 183;
   nguyenTo->dongViOnDinh[3] = 184;
   nguyenTo->dongViOnDinh[4] = 186;
   nguyenTo->dongViOnDinh[5] = 0;

   // ---- Reni
   nguyenTo = &(mangNguyenTo[RENI]);
   nguyenTo->so = 75;
   strcpy( nguyenTo->ten, "Reni" );
   strcpy( nguyenTo->kyHieu, "Re" );
   strcpy( nguyenTo->tenTrung, "铼" );
   nguyenTo->khoiLuong = 186.207f;
   strcpy( nguyenTo->soOxyHoa, "+2,+4,+6,+7" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 3459;
   nguyenTo->nhietDoSoi = 5869;
   nguyenTo->tiTrong = 21020;
   nguyenTo->nhietDung = 25.48f;
   nguyenTo->doAmDien = 1.9f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 760;
   nguyenTo->nangLuongIonHoa2 = 1260;
   nguyenTo->banKinh = 151;    // <------
   nguyenTo->dongViOnDinh[0] = 185;
   nguyenTo->dongViOnDinh[1] = 187;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Osimi
   nguyenTo = &(mangNguyenTo[OSIMI]);
   nguyenTo->so = 76;
   strcpy( nguyenTo->ten, "Osimi" );
   strcpy( nguyenTo->kyHieu, "Os" );
   strcpy( nguyenTo->tenTrung, "锇" );
   nguyenTo->khoiLuong = 190.23f;
   strcpy( nguyenTo->soOxyHoa, "+2,+3,+4,+8" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d⁶" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 3306;
   nguyenTo->nhietDoSoi = 5285;
   nguyenTo->tiTrong = 22590;
   nguyenTo->nhietDung = 24.70f;
   nguyenTo->doAmDien = 2.2f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 840;
   nguyenTo->nangLuongIonHoa2 = 1600;
   nguyenTo->banKinh = 144;    // <-----
   nguyenTo->dongViOnDinh[0] = 184;
   nguyenTo->dongViOnDinh[1] = 186;
   nguyenTo->dongViOnDinh[2] = 187;
   nguyenTo->dongViOnDinh[3] = 188;
   nguyenTo->dongViOnDinh[4] = 189;
   nguyenTo->dongViOnDinh[5] = 190;
   nguyenTo->dongViOnDinh[6] = 192;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Iriđi
   nguyenTo = &(mangNguyenTo[IRIDI]);
   nguyenTo->so = 77;
   strcpy( nguyenTo->ten, "Iriđi" );
   strcpy( nguyenTo->kyHieu, "Ir" );
   strcpy( nguyenTo->tenTrung, "铱" );
   nguyenTo->khoiLuong = 192.217f;
   strcpy( nguyenTo->soOxyHoa, "+3 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d⁷" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2739;
   nguyenTo->nhietDoSoi = 4701;
   nguyenTo->tiTrong = 22560;
   nguyenTo->nhietDung = 25.10f;
   nguyenTo->doAmDien = 2.2f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 880;
   nguyenTo->nangLuongIonHoa2 = 1600;
   nguyenTo->banKinh = 141;    // <------
   nguyenTo->dongViOnDinh[0] = 191;
   nguyenTo->dongViOnDinh[1] = 193;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Platin (Bạch Kim)
   nguyenTo = &(mangNguyenTo[PLATIN]);
   nguyenTo->so = 78;
   strcpy( nguyenTo->ten, "Platin" );
   strcpy( nguyenTo->kyHieu, "Pt" );
   strcpy( nguyenTo->tenTrung, "铂" );
   nguyenTo->khoiLuong = 195.084f;
   strcpy( nguyenTo->soOxyHoa, "+2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s¹4f¹⁴5d⁹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2041.4;
   nguyenTo->nhietDoSoi = 4098;
   nguyenTo->tiTrong = 21450;
   nguyenTo->nhietDung = 25.86f;
   nguyenTo->doAmDien = 2.2f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 870;
   nguyenTo->nangLuongIonHoa2 = 1791;
   nguyenTo->banKinh = 175;
   nguyenTo->dongViOnDinh[0] = 190;
   nguyenTo->dongViOnDinh[1] = 192;
   nguyenTo->dongViOnDinh[2] = 194;
   nguyenTo->dongViOnDinh[3] = 195;
   nguyenTo->dongViOnDinh[4] = 196;
   nguyenTo->dongViOnDinh[5] = 198;
   nguyenTo->dongViOnDinh[6] = 0;

   // ---- Vàng
   nguyenTo = &(mangNguyenTo[VANG]);
   nguyenTo->so = 79;
   strcpy( nguyenTo->ten, "Vàng" );
   strcpy( nguyenTo->kyHieu, "Au" );
   strcpy( nguyenTo->tenTrung, "金" );
   nguyenTo->khoiLuong = 196.9666f;
   strcpy( nguyenTo->soOxyHoa, "+1 +3" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s¹4f¹⁴5d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1337.33;
   nguyenTo->nhietDoSoi = 3129;
   nguyenTo->tiTrong = 19300;
   nguyenTo->nhietDung = 25.418f;
   nguyenTo->doAmDien = 2.54f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 890.1;
   nguyenTo->nangLuongIonHoa2 = 1980;
   nguyenTo->banKinh = 166;
   nguyenTo->dongViOnDinh[0] = 197;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Thủy Ngân
   nguyenTo = &(mangNguyenTo[THUY_NGAN]);
   nguyenTo->so = 80;
   strcpy( nguyenTo->ten, "Thủy Ngân" );
   strcpy( nguyenTo->kyHieu, "Hg" );
   strcpy( nguyenTo->tenTrung, "汞" );
   nguyenTo->khoiLuong = 200.592f;
   strcpy( nguyenTo->soOxyHoa, "+1 +2" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 234.32;
   nguyenTo->nhietDoSoi = 629.88;
   nguyenTo->tiTrong = 13534;
   nguyenTo->nhietDung = 27.983f;
   nguyenTo->doAmDien = 2.00f;
   nguyenTo->cauTrucTinhThe = BA_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 1007.1;
   nguyenTo->nangLuongIonHoa2 = 1810;
   nguyenTo->banKinh = 155;
   nguyenTo->dongViOnDinh[0] = 196;
   nguyenTo->dongViOnDinh[1] = 198;
   nguyenTo->dongViOnDinh[2] = 199;
   nguyenTo->dongViOnDinh[3] = 200;
   nguyenTo->dongViOnDinh[4] = 201;
   nguyenTo->dongViOnDinh[5] = 202;
   nguyenTo->dongViOnDinh[6] = 204;
   nguyenTo->dongViOnDinh[7] = 0;

   // ---- Tali
   nguyenTo = &(mangNguyenTo[TALI]);
   nguyenTo->so = 81;
   strcpy( nguyenTo->ten, "Tali" );
   strcpy( nguyenTo->kyHieu, "Tl" );
   strcpy( nguyenTo->tenTrung, "铊" );
   nguyenTo->khoiLuong = 204.3833f;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰6p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 577;
   nguyenTo->nhietDoSoi = 1746;
   nguyenTo->tiTrong = 11850;
   nguyenTo->nhietDung = 26.32f;
   nguyenTo->doAmDien = 1.62f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 589.4;
   nguyenTo->nangLuongIonHoa2 = 1971;
   nguyenTo->banKinh = 196;
   nguyenTo->dongViOnDinh[0] = 203;
   nguyenTo->dongViOnDinh[1] = 205;
   nguyenTo->dongViOnDinh[2] = 0;

   // ---- Chì
   nguyenTo = &(mangNguyenTo[CHI]);
   nguyenTo->so = 82;
   strcpy( nguyenTo->ten, "Chì" );
   strcpy( nguyenTo->kyHieu, "Pb" );
   strcpy( nguyenTo->tenTrung, "铅" );
   nguyenTo->khoiLuong = 207.2f;
   strcpy( nguyenTo->soOxyHoa, "+2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰6p²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 600.61;
   nguyenTo->nhietDoSoi = 2022;
   nguyenTo->tiTrong = 11340;
   nguyenTo->nhietDung = 26.650f;
   nguyenTo->doAmDien = 2.33f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 715.6;
   nguyenTo->nangLuongIonHoa2 = 1450.5;
   nguyenTo->banKinh = 202;
   nguyenTo->dongViOnDinh[0] = 204;
   nguyenTo->dongViOnDinh[1] = 206;
   nguyenTo->dongViOnDinh[2] = 207;
   nguyenTo->dongViOnDinh[3] = 208;
   nguyenTo->dongViOnDinh[4] = 0;

   // ---- Bitmut
   nguyenTo = &(mangNguyenTo[BITMUT]);
   nguyenTo->so = 83;
   strcpy( nguyenTo->ten, "Bitmut" );
   strcpy( nguyenTo->kyHieu, "Bi" );
   strcpy( nguyenTo->tenTrung, "铋" );
   nguyenTo->khoiLuong = 208.9804f;
   strcpy( nguyenTo->soOxyHoa, "+3 +5" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰6p³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 544.7f;
   nguyenTo->nhietDoSoi = 1837;
   nguyenTo->tiTrong = 11340;
   nguyenTo->nhietDung = 25.52f;
   nguyenTo->doAmDien = 2.02f;
   nguyenTo->cauTrucTinhThe = BA_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 703;
   nguyenTo->nangLuongIonHoa2 = 1610;
   nguyenTo->banKinh = 207;
   nguyenTo->dongViOnDinh[0] = 209;
   nguyenTo->dongViOnDinh[1] = 0;

   // ---- Poloni
   nguyenTo = &(mangNguyenTo[POLONI]);
   nguyenTo->so = 84;
   strcpy( nguyenTo->ten, "Poloni" );
   strcpy( nguyenTo->kyHieu, "Po" );
   strcpy( nguyenTo->tenTrung, "钋" );
   nguyenTo->khoiLuong = 209;
   strcpy( nguyenTo->soOxyHoa, "+2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰6p⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 527;
   nguyenTo->nhietDoSoi = 1235;
   nguyenTo->tiTrong = 9196;
   nguyenTo->nhietDung = 26.4f;
   nguyenTo->doAmDien = 2.0f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 812.1;
   nguyenTo->nangLuongIonHoa2 = 0.0f;
   nguyenTo->banKinh = 197;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Astatin
   nguyenTo = &(mangNguyenTo[ASTATIN]);
   nguyenTo->so = 85;
   strcpy( nguyenTo->ten, "Astatin" );
   strcpy( nguyenTo->kyHieu, "At" );
   strcpy( nguyenTo->tenTrung, "砹" );
   nguyenTo->khoiLuong = 210;
   strcpy( nguyenTo->soOxyHoa, "–1" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰6p⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 575;
   nguyenTo->nhietDoSoi = 610;
   nguyenTo->tiTrong = 9196;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 2.2f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 890;
   nguyenTo->nangLuongIonHoa2 = 0.0f;
   nguyenTo->banKinh = 202;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Rađon
   nguyenTo = &(mangNguyenTo[RADON]);
   nguyenTo->so = 86;
   strcpy( nguyenTo->ten, "Rađon" );
   strcpy( nguyenTo->kyHieu, "Rn" );
   strcpy( nguyenTo->tenTrung, "氡" );
   nguyenTo->khoiLuong = 222;
   strcpy( nguyenTo->soOxyHoa, "0" );
   strcpy( nguyenTo->cauHinhElecton, "[Xe] 6s²4f¹⁴5d¹⁰6p⁶" );
   nguyenTo->loai = KHI;
   nguyenTo->nhietDoChay = 202.0f;
   nguyenTo->nhietDoSoi = 211.3;
   nguyenTo->tiTrong = 9.73f;
   nguyenTo->nhietDung = 20.786f;
   nguyenTo->doAmDien = 2.2f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 1037;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 220;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Franxi
   nguyenTo = &(mangNguyenTo[FRANXI]);
   nguyenTo->so = 87;
   strcpy( nguyenTo->ten, "Franxi" );
   strcpy( nguyenTo->kyHieu, "Fr" );
   strcpy( nguyenTo->tenTrung, "钫" );
   nguyenTo->khoiLuong = 226;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 300;
   nguyenTo->nhietDoSoi = 950;
   nguyenTo->tiTrong = 1870;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 0.7f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 393;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 260;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Rađi
   nguyenTo = &(mangNguyenTo[RADI]);
   nguyenTo->so = 88;
   strcpy( nguyenTo->ten, "Rađi" );
   strcpy( nguyenTo->kyHieu, "Ra" );
   strcpy( nguyenTo->tenTrung, "镭" );
   nguyenTo->khoiLuong = 226;
   strcpy( nguyenTo->soOxyHoa, "+2" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 973;
   nguyenTo->nhietDoSoi = 2010;
   nguyenTo->tiTrong = 5500;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 0.9f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_KHOI;
   nguyenTo->nangLuongIonHoa1 = 509.3;
   nguyenTo->nangLuongIonHoa2 = 979;
   nguyenTo->banKinh = 283;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Actini
   nguyenTo = &(mangNguyenTo[ACTINI]);
   nguyenTo->so = 89;
   strcpy( nguyenTo->ten, "Actini" );
   strcpy( nguyenTo->kyHieu, "Ac" );
   strcpy( nguyenTo->tenTrung, "锕" );
   nguyenTo->khoiLuong = 227;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²6d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1323;
   nguyenTo->nhietDoSoi = 3471;
   nguyenTo->tiTrong = 10000;
   nguyenTo->nhietDung = 27.2;
   nguyenTo->doAmDien = 1.1f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 499;
   nguyenTo->nangLuongIonHoa2 = 1170;
   nguyenTo->banKinh = 215; // <----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Thôri
   nguyenTo = &(mangNguyenTo[THORI]);
   nguyenTo->so = 90;
   strcpy( nguyenTo->ten, "Thôri" );
   strcpy( nguyenTo->kyHieu, "Th" );
   strcpy( nguyenTo->tenTrung, "钍" );
   nguyenTo->khoiLuong = 232.0381f;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²6d²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2115;
   nguyenTo->nhietDoSoi = 5061;
   nguyenTo->tiTrong = 11700;
   nguyenTo->nhietDung = 26.23;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = LAP_PHUONG_TAM_MAT;
   nguyenTo->nangLuongIonHoa1 = 587;
   nguyenTo->nangLuongIonHoa2 = 1110;
   nguyenTo->banKinh = 206; // <----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Protactini
   nguyenTo = &(mangNguyenTo[PROTACTINI]);
   nguyenTo->so = 91;
   strcpy( nguyenTo->ten, "Protactini" );
   strcpy( nguyenTo->kyHieu, "Pa" );
   strcpy( nguyenTo->tenTrung, "镤" );
   nguyenTo->khoiLuong = 231.0359f;
   strcpy( nguyenTo->soOxyHoa, "+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f²6d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1841;
   nguyenTo->nhietDoSoi = 4300;
   nguyenTo->tiTrong = 15370;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.5f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 586;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 200; // <----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Urani
   nguyenTo = &(mangNguyenTo[URANI]);
   nguyenTo->so = 92;
   strcpy( nguyenTo->ten, "Urani" );
   strcpy( nguyenTo->kyHieu, "U" );
   strcpy( nguyenTo->tenTrung, "铀" );
   nguyenTo->khoiLuong = 238.0289f;
   strcpy( nguyenTo->soOxyHoa, "+3 +4 +6" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f³6d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1405.3f;
   nguyenTo->nhietDoSoi = 4404;
   nguyenTo->tiTrong = 19100;
   nguyenTo->nhietDung = 27.665f;
   nguyenTo->doAmDien = 1.38f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 186;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Neptuni
   nguyenTo = &(mangNguyenTo[NEPTUNI]);
   nguyenTo->so = 93;
   strcpy( nguyenTo->ten, "Neptuni" );
   strcpy( nguyenTo->kyHieu, "Np" );
   strcpy( nguyenTo->tenTrung, "镎" );
   nguyenTo->khoiLuong = 237;
   strcpy( nguyenTo->soOxyHoa, "+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f⁴6d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 910;
   nguyenTo->nhietDoSoi = 4273;
   nguyenTo->tiTrong = 20450;
   nguyenTo->nhietDung = 29.46f;
   nguyenTo->doAmDien = 1.36f;
   nguyenTo->cauTrucTinhThe = TRUC_THOI;
   nguyenTo->nangLuongIonHoa1 = 604.5f;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 190;  // <-----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Plutoni
   nguyenTo = &(mangNguyenTo[PLUTONI]);
   nguyenTo->so = 94;
   strcpy( nguyenTo->ten, "Plutoni" );
   strcpy( nguyenTo->kyHieu, "Pu" );
   strcpy( nguyenTo->tenTrung, "钚" );
   nguyenTo->khoiLuong = 244;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f⁶" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 912.5f;
   nguyenTo->nhietDoSoi = 3505;
   nguyenTo->tiTrong = 19816;
   nguyenTo->nhietDung = 35.5f;
   nguyenTo->doAmDien = 1.28f;
   nguyenTo->cauTrucTinhThe = DON_NGHIENG;
   nguyenTo->nangLuongIonHoa1 = 584.7f;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 187;  // <-----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Amerixi
   nguyenTo = &(mangNguyenTo[AMERIXI]);
   nguyenTo->so = 95;
   strcpy( nguyenTo->ten, "Amerixi" );
   strcpy( nguyenTo->kyHieu, "Am" );
   strcpy( nguyenTo->tenTrung, "镅" );
   nguyenTo->khoiLuong = 243;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f⁷" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1449;
   nguyenTo->nhietDoSoi = 2880;
   nguyenTo->tiTrong = 12000;
   nguyenTo->nhietDung = 62.7f;  // <---- ????
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = LUC_PHUONG;
   nguyenTo->nangLuongIonHoa1 = 587;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 180;  // <-----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Curi
   nguyenTo = &(mangNguyenTo[CURI]);
   nguyenTo->so = 96;
   strcpy( nguyenTo->ten, "Curi" );
   strcpy( nguyenTo->kyHieu, "Cm" );
   strcpy( nguyenTo->tenTrung, "锔" );
   nguyenTo->khoiLuong = 247;
   strcpy( nguyenTo->soOxyHoa, "+2 +3 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f⁷6d¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1613;
   nguyenTo->nhietDoSoi = 3383;
   nguyenTo->tiTrong = 13510;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 169;  // <-----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Beckeli
   nguyenTo = &(mangNguyenTo[BECKELI]);
   nguyenTo->so = 97;
   strcpy( nguyenTo->ten, "Beckeli" );
   strcpy( nguyenTo->kyHieu, "Bk" );
   strcpy( nguyenTo->tenTrung, "锫" );
   nguyenTo->khoiLuong = 247;
   strcpy( nguyenTo->soOxyHoa, "+3 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f⁹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1259;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = 14780;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 170;  // <-----
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Californi
   nguyenTo = &(mangNguyenTo[CALIFORNI]);
   nguyenTo->so = 98;
   strcpy( nguyenTo->ten, "Californi" );
   strcpy( nguyenTo->kyHieu, "Cf" );
   strcpy( nguyenTo->tenTrung, "锎" );
   nguyenTo->khoiLuong = 251;
   strcpy( nguyenTo->soOxyHoa, "+2 +3 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1173;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = 15100;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 608;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = kKHONG_BIET;
   nguyenTo->dongViOnDinh[0] = 0;

   // ---- Ensteni
   nguyenTo = &(mangNguyenTo[ENSTENI]);
   nguyenTo->so = 98;
   strcpy( nguyenTo->ten, "Ensteni" );
   strcpy( nguyenTo->kyHieu, "Es" );
   strcpy( nguyenTo->tenTrung, "锿" );
   nguyenTo->khoiLuong = 252;
   strcpy( nguyenTo->soOxyHoa, "+2 +3 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1133;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = 8840;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 608;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = kKHONG_BIET;

   // ---- Fecmi
   nguyenTo = &(mangNguyenTo[FECMI]);
   nguyenTo->so = 100;
   strcpy( nguyenTo->ten, "Fecmi" );
   strcpy( nguyenTo->kyHieu, "Fm" );
   strcpy( nguyenTo->tenTrung, "镄" );
   nguyenTo->khoiLuong = 257;
   strcpy( nguyenTo->soOxyHoa, "+2 +3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1800;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = 9700;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = kKHONG_BIET;

   // ---- Menđelevi
   nguyenTo = &(mangNguyenTo[MENDELEVI]);
   nguyenTo->so = 101;
   strcpy( nguyenTo->ten, "Menđelevi" );
   strcpy( nguyenTo->kyHieu, "Md" );
   strcpy( nguyenTo->tenTrung, "钔" );
   nguyenTo->khoiLuong = 258;
   strcpy( nguyenTo->soOxyHoa, "+2 +3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1100;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = 1.3f;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 635;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = kKHONG_BIET;

   // ---- Nobeli
   nguyenTo = &(mangNguyenTo[NOBELI]);
   nguyenTo->so = 102;
   strcpy( nguyenTo->ten, "Nobeli" );
   strcpy( nguyenTo->kyHieu, "No" );
   strcpy( nguyenTo->tenTrung, "锘" );
   nguyenTo->khoiLuong = 259;
   strcpy( nguyenTo->soOxyHoa, "+2 +3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 1100;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 641.6f;
   nguyenTo->nangLuongIonHoa2 = 1254.3f;
   nguyenTo->banKinh = kKHONG_BIET;

   // ---- Lorenxi
   nguyenTo = &(mangNguyenTo[LORENXI]);
   nguyenTo->so = 103;
   strcpy( nguyenTo->ten, "Lorenxi" );
   strcpy( nguyenTo->kyHieu, "Lr" );
   strcpy( nguyenTo->tenTrung, "铹" );
   nguyenTo->khoiLuong = 262;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴7p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 443.8f;
   nguyenTo->nangLuongIonHoa2 = 1428.0f;
   nguyenTo->banKinh = kKHONG_BIET;

   // ---- Rozofođi
   nguyenTo = &(mangNguyenTo[ROZOFODI]);
   nguyenTo->so = 104;
   strcpy( nguyenTo->ten, "Rozofođi" );
   strcpy( nguyenTo->kyHieu, "Rf" );
   strcpy( nguyenTo->tenTrung, "𬬻" );
   nguyenTo->khoiLuong = 267;
   strcpy( nguyenTo->soOxyHoa, "+4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 2400;
   nguyenTo->nhietDoSoi = 5800;
   nguyenTo->tiTrong = 23000;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 578;
   nguyenTo->nangLuongIonHoa2 = 1148;
   nguyenTo->banKinh = 157;   // <-----

   // ---- Đupni
   nguyenTo = &(mangNguyenTo[DUBNI]);
   nguyenTo->so = 105;
   strcpy( nguyenTo->ten, "Đupni" );
   strcpy( nguyenTo->kyHieu, "Db" );
   strcpy( nguyenTo->tenTrung, "𬭊" );
   nguyenTo->khoiLuong = 268;
   strcpy( nguyenTo->soOxyHoa, "+5" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 149;

   // ---- Sibogi
   nguyenTo = &(mangNguyenTo[SIBOGI]);
   nguyenTo->so = 106;
   strcpy( nguyenTo->ten, "Sibogi" );
   strcpy( nguyenTo->kyHieu, "Sg" );
   strcpy( nguyenTo->tenTrung, "𬭳" );
   nguyenTo->khoiLuong = 269;
   strcpy( nguyenTo->soOxyHoa, "+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 143;  // <----

   // ---- Bori
   nguyenTo = &(mangNguyenTo[BORI]);
   nguyenTo->so = 107;
   strcpy( nguyenTo->ten, "Bori" );
   strcpy( nguyenTo->kyHieu, "Bh" );
   strcpy( nguyenTo->tenTrung, "𬭛" );
   nguyenTo->khoiLuong = 270;
   strcpy( nguyenTo->soOxyHoa, "+7" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 740;
   nguyenTo->nangLuongIonHoa2 = 1690;
   nguyenTo->banKinh = 141;  // <----

   // ---- Hasi
   nguyenTo = &(mangNguyenTo[HASI]);
   nguyenTo->so = 108;
   strcpy( nguyenTo->ten, "Hasi" );
   strcpy( nguyenTo->kyHieu, "Hs" );
   strcpy( nguyenTo->tenTrung, "𬭶" );
   nguyenTo->khoiLuong = 269;
   strcpy( nguyenTo->soOxyHoa, "+8" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d⁶" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 740;
   nguyenTo->nangLuongIonHoa2 = 1690;
   nguyenTo->banKinh = 134;  // <----

   // ---- Méitneri
   nguyenTo = &(mangNguyenTo[MEITNERI]);
   nguyenTo->so = 109;
   strcpy( nguyenTo->ten, "Méitneri" );
   strcpy( nguyenTo->kyHieu, "Mt" );
   strcpy( nguyenTo->tenTrung, "鿏" );
   nguyenTo->khoiLuong = 278;
   strcpy( nguyenTo->soOxyHoa, "+3 +4 +6" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d⁷" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 129;  // <----

   // ---- Damastati
   nguyenTo = &(mangNguyenTo[DAMASTATI]);
   nguyenTo->so = 110;
   strcpy( nguyenTo->ten, "Damastati" );
   strcpy( nguyenTo->kyHieu, "Ds" );
   strcpy( nguyenTo->tenTrung, "𫟼" );
   nguyenTo->khoiLuong = 281;
   strcpy( nguyenTo->soOxyHoa, "+6" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d⁸" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 128;  // <----

   // ---- Roentgeni
   nguyenTo = &(mangNguyenTo[REONTGENI]);
   nguyenTo->so = 111;
   strcpy( nguyenTo->ten, "Roentgeni" );
   strcpy( nguyenTo->kyHieu, "Rg" );
   strcpy( nguyenTo->tenTrung, "𬬭" );
   nguyenTo->khoiLuong = 281;
   strcpy( nguyenTo->soOxyHoa, "+3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d⁹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 121;  // <----

   // ---- Copenixi
   nguyenTo = &(mangNguyenTo[COPENIXI]);
   nguyenTo->so = 112;
   strcpy( nguyenTo->ten, "Copenixi" );
   strcpy( nguyenTo->kyHieu, "Cn" );
   strcpy( nguyenTo->tenTrung, "鿔" );
   nguyenTo->khoiLuong = 285;
   strcpy( nguyenTo->soOxyHoa, "+2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = kKHONG_BIET;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 122;  // <----

   // ---- Nihoni
   nguyenTo = &(mangNguyenTo[NIHONI]);
   nguyenTo->so = 113;
   strcpy( nguyenTo->ten, "Nihoni" );
   strcpy( nguyenTo->kyHieu, "Nh" );
   strcpy( nguyenTo->tenTrung, "鿭" );
   nguyenTo->khoiLuong = 285;
   strcpy( nguyenTo->soOxyHoa, "+1" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰7p¹" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 700;
   nguyenTo->nhietDoSoi = 1400;
   nguyenTo->tiTrong = 18000;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = VO_DANG;
   nguyenTo->nangLuongIonHoa1 = 705;
   nguyenTo->nangLuongIonHoa2 = 1820;
   nguyenTo->banKinh = 176;  // <----

   // ---- Flerovi
   nguyenTo = &(mangNguyenTo[FLEROVI]);
   nguyenTo->so = 114;
   strcpy( nguyenTo->ten, "Flerovi" );
   strcpy( nguyenTo->kyHieu, "Fl" );
   strcpy( nguyenTo->tenTrung, "𫓧" );
   nguyenTo->khoiLuong = 289;
   strcpy( nguyenTo->soOxyHoa, "+2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰7p²" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 340;
   nguyenTo->nhietDoSoi = 420;
   nguyenTo->tiTrong = 14000;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 820;
   nguyenTo->nangLuongIonHoa2 = 1620;
   nguyenTo->banKinh = 143;  // <----

   // ---- Moscovi
   nguyenTo = &(mangNguyenTo[MOSCOVI]);
   nguyenTo->so = 115;
   strcpy( nguyenTo->ten, "Moscovi" );
   strcpy( nguyenTo->kyHieu, "Mc" );
   strcpy( nguyenTo->tenTrung, "镆" );
   nguyenTo->khoiLuong = 289;
   strcpy( nguyenTo->soOxyHoa, "+1 +3" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰7p³" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 700;
   nguyenTo->nhietDoSoi = 1400;
   nguyenTo->tiTrong = 11000;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = 538;
   nguyenTo->nangLuongIonHoa2 = 2055;
   nguyenTo->banKinh = 162;  // <----

   // ---- Livemori
   nguyenTo = &(mangNguyenTo[LIVEMORI]);
   nguyenTo->so = 116;
   strcpy( nguyenTo->ten, "Livemori" );
   strcpy( nguyenTo->kyHieu, "Lv" );
   strcpy( nguyenTo->tenTrung, "𫟷" );
   nguyenTo->khoiLuong = 293;
   strcpy( nguyenTo->soOxyHoa, "+2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰7p⁴" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 700;
   nguyenTo->nhietDoSoi = 1400;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 175;  // <----

   // ---- Tenexin
   nguyenTo = &(mangNguyenTo[TENEXIN]);
   nguyenTo->so = 117;
   strcpy( nguyenTo->ten, "Tenexin" );
   strcpy( nguyenTo->kyHieu, "Ts" );
   strcpy( nguyenTo->tenTrung, "鿬" );
   nguyenTo->khoiLuong = 294;
   strcpy( nguyenTo->soOxyHoa, "+1 +3 +5 +7" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰7p⁵" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = 700;
   nguyenTo->nhietDoSoi = 1400;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = VO_DANG;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 165;  // <----

   // ---- Oganeson
   nguyenTo = &(mangNguyenTo[OGANESON]);
   nguyenTo->so = 118;
   strcpy( nguyenTo->ten, "Ôganeson" );
   strcpy( nguyenTo->kyHieu, "Og" );
   strcpy( nguyenTo->tenTrung, "鿫" );
   nguyenTo->khoiLuong = 294;
   strcpy( nguyenTo->soOxyHoa, "0 +2 +4" );
   strcpy( nguyenTo->cauHinhElecton, "[Rn] 7s²5f¹⁴6d¹⁰7p⁶" );
   nguyenTo->loai = RAN;
   nguyenTo->nhietDoChay = kKHONG_BIET;
   nguyenTo->nhietDoSoi = 350;
   nguyenTo->tiTrong = kKHONG_BIET;
   nguyenTo->nhietDung = kKHONG_BIET;
   nguyenTo->doAmDien = kKHONG_BIET;
   nguyenTo->cauTrucTinhThe = VO_DANG;
   nguyenTo->nangLuongIonHoa1 = kKHONG_BIET;
   nguyenTo->nangLuongIonHoa2 = kKHONG_BIET;
   nguyenTo->banKinh = 157;  // <----
}

#pragma mark ==== vẽ ô
//    0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3
//    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// 0  *                                                             *
// 1  * *                                                 * * * * * *
// 2  * *                                                 * * * * * *
// 3  * *                             * * * * * * * * * * * * * * * *
// 4  * *                             * * * * * * * * * * * * * * * *
// 5  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 6  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// (0; 0)      (1; 0)
// +------------+
// |    tên     |
// +------------+
// |            |
// |            |
// |            |
// |            |
// +------------+
// (0; 1)       (1; !)

#define kTEN__VI_TRI_X  0.50f
#define kTEN__VI_TRI_Y  0.15f

#define kSO_NGUYEN_TU__VI_TRI_X  0.70f
#define kSO_NGUYEN_TU__VI_TRI_Y  0.36f

#define kKY_HIEU__VI_TRI_X  0.19f
#define kKY_HIEU__VI_TRI_Y  0.36f

#define kTEN_TRUNG__VI_TRI_X  0.95f
#define kTEN_TRUNG__VI_TRI_Y  0.36f

#define kSO_OXI_HOA__VI_TRI_X  0.05f
#define kSO_OXI_HOA__VI_TRI_Y  0.48f

#define kCAU_HINH_ELECTRON__VI_TRI_X  0.50f
#define kCAU_HINH_ELECTRON__VI_TRI_Y  0.81f

#define kNHIET_DUNG__VI_TRI_X  0.95f
#define kNHIET_DUNG__VI_TRI_Y  0.60f

#define kNHIET_DO_CHAY__VI_TRI_X  0.40f
#define kNHIET_DO_CHAY__VI_TRI_Y  0.60f

#define kNHIET_DO_SOI__VI_TRI_X  0.40f
#define kNHIET_DO_SOI__VI_TRI_Y  0.70f

#define kTI_TRONG__VI_TRI_X  0.95f
#define kTI_TRONG__VI_TRI_Y  0.70f

#define kKHOI_LUONG__VI_TRI_X  0.05f
#define kKHOI_LUONG__VI_TRI_Y  0.93f

#define kDO_AM_DIEN__VI_TRI_X  0.95f
#define kDO_AM_DIEN__VI_TRI_Y  0.95f

#define kDONG_VI__VI_TRI_X  0.02f
#define kDONG_VI__VI_TRI_Y  0.985f

void vanBanNguyenTo( FILE *tep, NguyenTo *nguyenTo, float gocX, float gocY, float coO_ngang, float coO_doc, unsigned int mauVanBan, float phongTo ) {

   char xauSo[32];

   // ---- tên
   float gocX_dacTrung = gocX + coO_ngang*kTEN__VI_TRI_X;
   float gocY_dacTrung = gocY + coO_doc*kTEN__VI_TRI_Y;
   vanBan_ngang( tep, nguyenTo->ten, gocX_dacTrung, gocY_dacTrung, "Arial", 25.0f*phongTo, "Bold",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   
   // ---- ký hiệu
   gocX_dacTrung = gocX + coO_ngang*kKY_HIEU__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kKY_HIEU__VI_TRI_Y;
   vanBan_ngang( tep, nguyenTo->kyHieu, gocX_dacTrung, gocY_dacTrung, "Arial", 30.0f*phongTo, "Bold",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   
   // ---- số nguyên tử
   gocX_dacTrung = gocX + coO_ngang*kSO_NGUYEN_TU__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kSO_NGUYEN_TU__VI_TRI_Y;
   sprintf( xauSo, "%d", nguyenTo->so );
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 25.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   
   // ---- tên tiếng Trung
   gocX_dacTrung = gocX + coO_ngang*kTEN_TRUNG__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kTEN_TRUNG__VI_TRI_Y;
   vanBan_ngang( tep, nguyenTo->tenTrung, gocX_dacTrung, gocY_dacTrung, "Arial", 25.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   
   // ---- số oxy hóa
   gocX_dacTrung = gocX + coO_ngang*kSO_OXI_HOA__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kSO_OXI_HOA__VI_TRI_Y;
   vanBan_ngang( tep, nguyenTo->soOxyHoa, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Bold",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
   
   // ---- cầu hình electron
   gocX_dacTrung = gocX + coO_ngang*kCAU_HINH_ELECTRON__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kCAU_HINH_ELECTRON__VI_TRI_Y;
   vanBan_ngang( tep, nguyenTo->cauHinhElecton, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   
   // ---- nhiệt độ chảy và sôi
   gocX_dacTrung = gocX + coO_ngang*kNHIET_DO_CHAY__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kNHIET_DO_CHAY__VI_TRI_Y;
   float nhietDo = nguyenTo->nhietDoChay;
   if( nhietDo == kKHONG_BIET )
      sprintf( xauSo, "-" );
   else if( nhietDo > 273.0f ) // rắn
      sprintf( xauSo, "%5.0f", nhietDo );
   else
      sprintf( xauSo, "%5.2f", nhietDo );
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   
   nhietDo = nguyenTo->nhietDoSoi;
   if( nhietDo == kKHONG_BIET )
      sprintf( xauSo, "-" );
   else if( nhietDo > 273.0f ) // rắn
      sprintf( xauSo, "%5.0f", nhietDo );
   else
      sprintf( xauSo, "%5.2f", nhietDo );
   gocY_dacTrung = gocY + coO_doc*kNHIET_DO_SOI__VI_TRI_Y;
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   
   // ---- nhiệt dung
   gocX_dacTrung = gocX + coO_ngang*kNHIET_DUNG__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kNHIET_DUNG__VI_TRI_Y;
   float nhietDung = nguyenTo->nhietDung;
   if( nhietDung == kKHONG_BIET )
      sprintf( xauSo, "-" );
   else
      sprintf( xauSo, "%5.2f", nhietDung );
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   
   // ---- tỉ trọng
   gocX_dacTrung = gocX + coO_ngang*kTI_TRONG__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kTI_TRONG__VI_TRI_Y;
   float tiTrong = nguyenTo->tiTrong;
   if( tiTrong == kKHONG_BIET )
      sprintf( xauSo, "-" );
   else if( tiTrong > 10.0f ) // rắn
      sprintf( xauSo, "%5.0f", tiTrong );
   else
      sprintf( xauSo, "%5.3f", tiTrong );
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   
   // ---- khối lượng nguyên tử
   gocX_dacTrung = gocX + coO_ngang*kKHOI_LUONG__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kKHOI_LUONG__VI_TRI_Y;
   float so = nguyenTo->khoiLuong;
   if( so == roundf( so ) )  // cho nguyên tố rã bức xạ
      sprintf( xauSo, "(%3.0f)", nguyenTo->khoiLuong );
   else
      sprintf( xauSo, "%3.4f", nguyenTo->khoiLuong );
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
   
   // ---- độ âm điện
   gocX_dacTrung = gocX + coO_ngang*0.95f;
   float doAmDien = nguyenTo->doAmDien;
   if( doAmDien == kKHONG_BIET )
      sprintf( xauSo, "-" );
   else
      sprintf( xauSo, "%3.2f", doAmDien);
   vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 14.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   // ---- đồng vị ổ định
   gocX_dacTrung = gocX + coO_ngang*kDONG_VI__VI_TRI_X;
   gocY_dacTrung = gocY + coO_doc*kDONG_VI__VI_TRI_Y;
   unsigned char dongVi = nguyenTo->dongViOnDinh[0];
   if( dongVi == 0 )
      vanBan_ngang( tep, "- - -", gocX_dacTrung, gocY_dacTrung, "Arial", 6.0f*phongTo, "Normal",
                   kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
   else {
      unsigned char chiSo = 1;
      while( dongVi ) {  // chuỗi đồng vị ổn định kết thúc bởi số không
         sprintf( xauSo, "%3d ", dongVi );
         vanBan_ngang( tep, xauSo, gocX_dacTrung, gocY_dacTrung, "Arial", 6.0f*phongTo, "Normal",
                kDUNG, mauVanBan, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
         gocX_dacTrung += coO_ngang*0.1f*phongTo;
         dongVi = nguyenTo->dongViOnDinh[chiSo];
         chiSo++;
      }
   }

}

void veNen( FILE *tep, float gocX, float gocY, float coO_ngang, float coO_doc, unsigned int mauNenToanBo, unsigned int mauNenKyHieu ) {


   chuNhat( tep, gocX, gocY, coO_ngang, coO_doc, kDUNG, mauNenToanBo, 1.0f,
           kDUNG, 1.0f, 0x987654, 1.0f );
   // ---- nền tên
   chuNhat( tep, gocX, gocY, coO_ngang, coO_doc*0.20f, kDUNG, 0x000000, 0.5f,
           kSAI, 0.0f, 0x000000, 0.0f );
   // ---- nền ký hiệu và số
   chuNhat( tep, gocX, gocY + coO_doc*0.20f, coO_ngang*0.38f, coO_doc*0.20f, kDUNG, mauNenKyHieu, 0.7f,
           kSAI, 0.0f, 0x000000, 0.0f );
   // ---- nền nhiệt độ
   chuNhat( tep, gocX, gocY + 0.5f*coO_doc, coO_ngang*0.45f, coO_doc*0.23f, kDUNG, 0xffffff, 0.15f,
           kSAI, 0.0f, 0x000000, 0.0f );
   // ---- nền cấu hình electron
   chuNhat( tep, gocX, gocY + 0.73f*coO_doc, coO_ngang, coO_doc*0.12f, kDUNG, 0x000000, 0.25f,
           kSAI, 0.0f, 0x000000, 0.0f );
   // ---- nền độ âm điện
   chuNhat( tep, gocX + 0.70f*coO_ngang, gocY + 0.85f*coO_doc, coO_ngang*0.30f, coO_doc*0.10, kDUNG, 0xffffff, 0.3f,
           kSAI, 0.0f, 0x000000, 0.0f );
   
   // ---- những đồng vị ổn định
   chuNhat( tep, gocX, gocY + 0.95f*coO_doc, coO_ngang, coO_doc*0.05, kDUNG, 0xffffff, 0.15f,
           kSAI, 0.0f, 0x000000, 0.0f );
}


#define kVAN_BAN_DAC_TRUNG__VI_TRI_X  0.50f
#define kVAN_BAN_DAC_TRUNG__VI_TRI_Y -0.35f
#define kVAN_BAN_TEN__VI_TRI_X -0.20f
#define kVAN_BAN_TEN__VI_TRI_Y  0.00f
#define kVAN_BAN_KY_HIEU__VI_TRI_X -0.20f
#define kVAN_BAN_KY_HIEU__VI_TRI_Y  0.30f
#define kVAN_BAN_SO__VI_TRI_X  1.20f
#define kVAN_BAN_SO__VI_TRI_Y  0.00f
#define kVAN_BAN_TEN_TRUNG__VI_TRI_X  1.20f
#define kVAN_BAN_TEN_TRUNG__VI_TRI_Y  0.25f
#define kVAN_BAN_SO_OXY_HOA__VI_TRI_X -0.20f
#define kVAN_BAN_SO_OXY_HOA__VI_TRI_Y  0.48f
#define kVAN_BAN_NHIET_DO_CHAY__VI_TRI_X -0.20f
#define kVAN_BAN_NHIET_DO_CHAY__VI_TRI_Y 0.65f
#define kVAN_BAN_NHIET_DO_SOI__VI_TRI_X -0.20f
#define kVAN_BAN_NHIET_DO_SOI__VI_TRI_Y  0.82f
#define kVAN_BAN_NHIET_DUNG__VI_TRI_X  1.20f
#define kVAN_BAN_NHIET_DUNG__VI_TRI_Y  0.50f
#define kVAN_BAN_TI_TRONG__VI_TRI_X  1.20f
#define kVAN_BAN_TI_TRONG__VI_TRI_Y  0.70f
#define kVAN_BAN_CAU_HINH_ELECTRON__VI_TRI_X  1.20f
#define kVAN_BAN_CAU_HINH_ELECTRON__VI_TRI_Y  0.90f
#define kVAN_BAN_KHOI_LUONG__VI_TRI_X  0.00f
#define kVAN_BAN_KHOI_LUONG__VI_TRI_Y  1.15f
#define kVAN_BAN_DO_AM_DIEN__VI_TRI_X  1.00f
#define kVAN_BAN_DO_AM_DIEN__VI_TRI_Y  1.15f
#define kVAN_BAN_DONG_VI__VI_TRI_X 0.50f
#define kVAN_BAN_DONG_VI__VI_TRI_Y 1.35f

void soDoMoTaDacTrung( FILE *tep, NguyenTo *nguyenTo, float gocX, float gocY, float coO_ngang, float coO_doc, float phongTo ) {
   
   // ---- nền
   // ---- vẽ chữ nhật nền
   veNen( tep, gocX, gocY, coO_ngang, coO_doc, 0x404040, 0xc0c0c0 );
   // ---- văn bản cho nguyên tố ví dụ
   vanBanNguyenTo( tep, nguyenTo, gocX, gocY, coO_ngang, coO_doc, 0xffffff, phongTo );

   // ---- đặc trưng
   float viTriX = gocX + coO_ngang*kVAN_BAN_DAC_TRUNG__VI_TRI_X;
   float viTriY = gocY + coO_doc*kVAN_BAN_DAC_TRUNG__VI_TRI_Y;
   vanBan_ngang( tep, "Đặc Trưng", viTriX, viTriY, "Arial", 35.0f, "Bold", kDUNG, 0x000000, 2.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   viTriY += 0.15f*coO_doc;
   vanBan_ngang( tep, "(P = 101,3 kPa, T = 273 K)", viTriX, viTriY, "Arial", 20.0f, "Bold", kDUNG, 0x000000, 2.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   

   // ---- tên
   viTriX = gocX + coO_ngang*kVAN_BAN_TEN__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_TEN__VI_TRI_Y;
   vanBan_ngang( tep, "Tên", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   float x1 = gocX + coO_ngang*0.25f;
   float y1 = gocY + coO_doc*0.10f;
   float x2 = gocX + coO_ngang*(kVAN_BAN_TEN__VI_TRI_X + 0.02f);
   float y2 = gocY + coO_doc*(kVAN_BAN_TEN__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   // ---- ký hiểu
   viTriX = gocX + coO_ngang*kVAN_BAN_KY_HIEU__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_KY_HIEU__VI_TRI_Y;
   vanBan_ngang( tep, "Ký hiệu", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   x1 = gocX + coO_ngang*0.05f;
   y1 = gocY + coO_doc*0.30f;
   x2 = gocX + coO_ngang*(kVAN_BAN_KY_HIEU__VI_TRI_X + 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_KY_HIEU__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   // ---- số nguyên tử
   viTriX = gocX + coO_ngang*kVAN_BAN_SO__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_SO__VI_TRI_Y;
   vanBan_ngang( tep, "Số Nguyên Tử", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );

   x1 = gocX + coO_ngang*0.60f;
   y1 = gocY + coO_doc*0.30f;
   x2 = gocX + coO_ngang*(kVAN_BAN_SO__VI_TRI_X - 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_SO__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   // ---- tên Trung
   x1 = gocX + coO_ngang*0.90f;
   y1 = gocY + coO_doc*0.30f;
   x2 = gocX + coO_ngang*(kVAN_BAN_TEN_TRUNG__VI_TRI_X - 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_TEN_TRUNG__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_TEN_TRUNG__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_TEN_TRUNG__VI_TRI_Y;
   vanBan_ngang( tep, "Tên tiếng Trung", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );

   // ---- số oxy hóa
   x1 = gocX + coO_ngang*0.1f;
   y1 = gocY + coO_doc*0.45f;
   x2 = gocX + coO_ngang*(kVAN_BAN_SO_OXY_HOA__VI_TRI_X + 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_SO_OXY_HOA__VI_TRI_Y - 0.02);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_SO_OXY_HOA__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_SO_OXY_HOA__VI_TRI_Y;
   vanBan_ngang( tep, "Số oxy hóa phổ biến", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   // ---- nhiệt độ chảy
   x1 = gocX + coO_ngang*0.1f;
   y1 = gocY + coO_doc*0.60f;
   x2 = gocX + coO_ngang*(kVAN_BAN_NHIET_DO_CHAY__VI_TRI_X + 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_NHIET_DO_CHAY__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_NHIET_DO_CHAY__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_NHIET_DO_CHAY__VI_TRI_Y;
   vanBan_ngang( tep, "Nhiệt độ chảy (K)", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   // ---- nhiệt độ sôi
   x1 = gocX + coO_ngang*0.1f;
   y1 = gocY + coO_doc*0.70f;
   x2 = gocX + coO_ngang*(kVAN_BAN_NHIET_DO_SOI__VI_TRI_X + 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_NHIET_DO_SOI__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_NHIET_DO_SOI__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_NHIET_DO_SOI__VI_TRI_Y;
   vanBan_ngang( tep, "Nhiệt độ sôi (K)", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   // ---- nhiệt dụng
   x1 = gocX + coO_ngang*0.90f;
   y1 = gocY + coO_doc*0.58f;
   x2 = gocX + coO_ngang*(kVAN_BAN_NHIET_DUNG__VI_TRI_X - 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_NHIET_DUNG__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );
   
   viTriX = gocX + coO_ngang*kVAN_BAN_NHIET_DUNG__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_NHIET_DUNG__VI_TRI_Y;
   vanBan_ngang( tep, "Nhiệt Dung (kJ/(mol•K))", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
   
   // ---- tỉ trọng
   x1 = gocX + coO_ngang*0.90f;
   y1 = gocY + coO_doc*0.68f;
   x2 = gocX + coO_ngang*(kVAN_BAN_TI_TRONG__VI_TRI_X - 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_TI_TRONG__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );
   
   viTriX = gocX + coO_ngang*kVAN_BAN_TI_TRONG__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_TI_TRONG__VI_TRI_Y;
   vanBan_ngang( tep, "Tỉ trọng (kg/m³)", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );

   // ---- Cấu hình elentron
   x1 = gocX + coO_ngang*0.70f;
   y1 = gocY + coO_doc*0.80f;
   x2 = gocX + coO_ngang*(kVAN_BAN_CAU_HINH_ELECTRON__VI_TRI_X - 0.02f);
   y2 = gocY + coO_doc*(kVAN_BAN_CAU_HINH_ELECTRON__VI_TRI_Y - 0.02f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_CAU_HINH_ELECTRON__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_CAU_HINH_ELECTRON__VI_TRI_Y;
   vanBan_ngang( tep, "Cấu hình elentron", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );

   // ---- Khối lượng nguyên tử
   x1 = gocX + coO_ngang*0.10f;
   y1 = gocY + coO_doc*0.90f;
   x2 = gocX + coO_ngang*(kVAN_BAN_KHOI_LUONG__VI_TRI_X - 0.50f);
   y2 = gocY + coO_doc*(kVAN_BAN_KHOI_LUONG__VI_TRI_Y - 0.08f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_KHOI_LUONG__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_KHOI_LUONG__VI_TRI_Y;
   vanBan_ngang( tep, "Khối lượng nguyên tử", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );

   // ---- độ âm điện
   x1 = gocX + coO_ngang*0.9f;
   y1 = gocY + coO_doc*0.90f;
   x2 = gocX + coO_ngang*(kVAN_BAN_DO_AM_DIEN__VI_TRI_X + 0.2f);
   y2 = gocY + coO_doc*(kVAN_BAN_DO_AM_DIEN__VI_TRI_Y - 0.08f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );

   viTriX = gocX + coO_ngang*kVAN_BAN_DO_AM_DIEN__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_DO_AM_DIEN__VI_TRI_Y;
   vanBan_ngang( tep, "Độ âm điện", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
   
   // ---- những đồng vị ổn định
   x1 = gocX + coO_ngang*0.08f;
   y1 = gocY + coO_doc*0.99f;
   x2 = gocX + coO_ngang*kVAN_BAN_DONG_VI__VI_TRI_X;
   y2 = gocY + coO_doc*(kVAN_BAN_DONG_VI__VI_TRI_Y - 0.08f);
   duong( tep, x1, y1, x2, y2, 2.0f, 0x0000ff, 0.5f );
   
   viTriX = gocX + coO_ngang*kVAN_BAN_DONG_VI__VI_TRI_X;
   viTriY = gocY + coO_doc*kVAN_BAN_DONG_VI__VI_TRI_Y;
   vanBan_ngang( tep, "Những đồng vị ổ định (hay > 10¹⁰ năm)", viTriX, viTriY, "Arial", 25.0f, "Normal",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
}


void veBangTuanHoanDungCoO( FILE *tep, float coO_ngang, float coO_doc, NguyenTo *mangNguyenTo, float dichX, float dichY, float giuaNgang, float giuaDoc ) {
   
   unsigned char soNguyenTo = 0;
   float gocX = dichX;
   float gocY = dichY;
   char xauSo[32];
   
   float mangMauNenToanBo[8] = {
      0.07f, 0.15f, 0.28f, 1.00f,
      1.00f, 0.00f, 0.18f, 1.00f,
   };

   float mangMauNenKyHieu[8] = {
      0.07f, 0.50f, 0.70f, 1.00f,
      1.00f, 0.35f, 0.07f, 1.00f,
   };
   
   // ---- màu cho khí trơ
   float mangMauNenKhiTro[8] = {
      0.25f, 0.35f, 0.33f, 1.00f,
      0.00f, 0.50f, 0.45f, 1.00f,
   };

   float beRongVungTo = kBE_RONG__KHO - kLE_TRAI - kLE_PHAI;
   float beCaoVungTo = kBE_CAO__KHO - kLE_DUOI - kLE_TREN;
   
   // ---- tiêu đề
   vanBan_ngang( tep, "Bảng Tuần Hoàn", kBE_RONG__KHO >> 1, kLE_TREN, "Arial", 120.0f, "Bold",
                kDUNG, 0x000000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );

   while( soNguyenTo < kSO_LUONG__NGUYEN_TO ) {

      
      if( soNguyenTo == HELI )
         gocX += 30.0f*(coO_ngang + giuaNgang);
      else if( soNguyenTo == LITI ) {
         gocX = dichX;
         gocY += coO_doc + giuaDoc;
      }
      else if( soNguyenTo == BO )
         gocX += 24.0f*(coO_ngang + giuaNgang);
      else if( soNguyenTo == NATRI ) {
         gocX = dichX;
         gocY += coO_doc + giuaDoc;
      }
      else if( soNguyenTo == NHOM )
         gocX += 24.0f*(coO_ngang + giuaNgang);
      // ----
      else if( soNguyenTo == KALI ) {
         gocX = dichX;
         gocY += coO_doc + giuaDoc;
      }
      else if( soNguyenTo == SCANDI )
         gocX += 14.0f*(coO_ngang + giuaNgang);
      // ----
      else if( soNguyenTo == RUBIDI ) {
         gocX = dichX;
         gocY += coO_doc + giuaDoc;
      }
      else if( soNguyenTo == YTRI )
         gocX += 14.0f*(coO_ngang + giuaNgang);
      // ----
      else if( soNguyenTo == XESI ) {
         gocX = dichX;
         gocY += coO_doc + giuaDoc;
      }
      else if( soNguyenTo == FRANXI ) {
         gocX = dichX;
         gocY += coO_doc + giuaDoc;
      }
      
      // ---- chọn màu
      float phanSoX = gocX/beRongVungTo;
      float phanSoY = gocY/beCaoVungTo;
      if( phanSoX > 1.0f )
         phanSoX = 1.0f;
      if( phanSoY > 1.0f )
         phanSoY = 1.0f;

      // ---- tính phân số cho tô màu
      float phanSo = 0.0f;
      if( (soNguyenTo == HELI) || (soNguyenTo == NEON) || (soNguyenTo == ARGON) || (soNguyenTo == KRYPTON) ||
         (soNguyenTo == XENON) || (soNguyenTo == RADON) || (soNguyenTo == OGANESON) ) {
         phanSo = 1.0f - phanSoY;
      }
      else
         phanSo = (phanSoX + (1.0f - phanSoY))*0.5f;

      float nghichPhanSo = 1.0f - phanSo;
      
      // ---- tính màu
      float mauDo;
      float mauLuc;
      float mauXanh;
      if( (soNguyenTo == HELI) || (soNguyenTo == NEON) || (soNguyenTo == ARGON) || (soNguyenTo == KRYPTON) ||
         (soNguyenTo == XENON) || (soNguyenTo == RADON) || (soNguyenTo == OGANESON) ) {
         mauDo = nghichPhanSo*mangMauNenKhiTro[0] + phanSo*mangMauNenKhiTro[4];
         mauLuc = nghichPhanSo*mangMauNenKhiTro[1] + phanSo*mangMauNenKhiTro[5];
         mauXanh = nghichPhanSo*mangMauNenKhiTro[2] + phanSo*mangMauNenKhiTro[6];
      }
      else {
         mauDo = nghichPhanSo*mangMauNenToanBo[0] + phanSo*mangMauNenToanBo[4];
         mauLuc = nghichPhanSo*mangMauNenToanBo[1] + phanSo*mangMauNenToanBo[5];
         mauXanh = nghichPhanSo*mangMauNenToanBo[2] + phanSo*mangMauNenToanBo[6];
      }
      
      // ---- đổi sang phạm vi 0x00 đến 0xff
      unsigned char mauDoNenToanBo = (unsigned char)(mauDo*255.0f);
      unsigned char mauLucNenToanBo = (unsigned char)(mauLuc*255.0f);
      unsigned char mauXanhNenToanBo = (unsigned char)(mauXanh*255.0f);
      unsigned int mauNenToanBo = mauDoNenToanBo << 16 | mauLucNenToanBo << 8 | mauXanhNenToanBo;

      // ---- tính màu nền ký hiệu vaô số
      mauDo = nghichPhanSo*mangMauNenKyHieu[0] + phanSo*mangMauNenKyHieu[4];
      mauLuc = nghichPhanSo*mangMauNenKyHieu[1] + phanSo*mangMauNenKyHieu[5];
      mauXanh = nghichPhanSo*mangMauNenKyHieu[2] + phanSo*mangMauNenKyHieu[6];
      unsigned char mauDoNenKyHieu = (unsigned char)(mauDo*255.0f);
      unsigned char mauLucNenKyHieu = (unsigned char)(mauLuc*255.0f);
      unsigned char mauXanhNenKyHieu = (unsigned char)(mauXanh*255.0f);
      unsigned int mauNenKyHieu = mauDoNenKyHieu << 16 | mauLucNenKyHieu << 8 | mauXanhNenKyHieu;
   
      // ==== nền
      // ---- vẽ chữ nhật nền
      veNen( tep, gocX, gocY, coO_ngang, coO_doc, mauNenToanBo, mauNenKyHieu );

      // ==== văn bản
      NguyenTo *nguyenTo = &(mangNguyenTo[soNguyenTo]);
      vanBanNguyenTo( tep, nguyenTo, gocX, gocY, coO_ngang, coO_doc, 0xffffff, 1.0f );
      
      // ---- nguyên tố tiếp
      gocX += coO_ngang + giuaNgang;
      soNguyenTo++;
   }
   
   // ==== số chu kỳ
   // ----- trái
   gocY = dichY + coO_doc*0.6f;
   gocX = dichX - 30.0f;
   unsigned char chuKy = 0;
   while( chuKy < 7 ) {
      sprintf( xauSo, "%d", chuKy+1 );
      vanBan_ngang( tep, xauSo, gocX, gocY, "Arial", 50.0f, "Normal",
                   kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );

      gocY += coO_doc + giuaDoc;
      chuKy++;
   }
   // ---- phải
   gocY = dichY + coO_doc*0.6f;
   gocX = kBE_RONG__KHO - kLE_PHAI + 30.0f;
   chuKy = 0;
   while( chuKy < 7 ) {
      sprintf( xauSo, "%d", chuKy+1 );
      vanBan_ngang( tep, xauSo, gocX, gocY, "Arial", 50.0f, "Normal",
                   kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
      
      gocY += coO_doc + giuaDoc;
      chuKy++;
   }
   
   // ==== số nhóm
   gocX = dichX + coO_ngang*0.5f;
   gocY = dichY + (coO_doc + giuaDoc)*7.3f;
   unsigned char nhom = 0;
   while( nhom < 18 ) {
      sprintf( xauSo, "%d", nhom+1 );
      vanBan_ngang( tep, xauSo, gocX, gocY, "Arial", 50.0f, "Normal",
                   kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
      if( nhom == 1 )
         gocX += (coO_ngang + giuaNgang)*15.0f; // nhảy qua khối f
      else
         gocX += coO_ngang + giuaNgang;

      nhom++;
   }

   // ==== các khối
   // ---- s
   gocX = dichX + (coO_ngang + giuaNgang);
   gocY = dichY - 0.2f*(coO_doc + giuaDoc);
   vanBan_ngang( tep, "Khối s", gocX, gocY, "Arial", 50.0f, "Normal",
                kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );

   // ---- f
   gocX = dichX + (2.0f + 7.0f)*(coO_ngang + giuaNgang);
   gocY = dichY + 4.8f*(coO_doc + giuaDoc);
   vanBan_ngang( tep, "Khối f", gocX, gocY, "Arial", 50.0f, "Normal",
                kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   
   // ---- d
   gocX = dichX + (2.0f + 14.0f + 5.0f)*(coO_ngang + giuaNgang);
   gocY = dichY + 2.8f*(coO_doc + giuaDoc);
   vanBan_ngang( tep, "Khối d", gocX, gocY, "Arial", 50.0f, "Normal",
                kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );

   // ---- p
   gocX = dichX + (2.0f + 14.0f + 10.0f + 3.0f)*(coO_ngang + giuaNgang);
   gocY = dichY + 0.8f*(coO_doc + giuaDoc);
   vanBan_ngang( tep, "Khối p", gocX, gocY, "Arial", 50.0f, "Normal",
                kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "middle" );
   
   // ==== Mô tả về đặc trưng cho nguyên tố Z > 94
   gocX = kBE_RONG__KHO >> 1;
   gocY = dichY + 7.5f*(coO_doc + giuaDoc);
   vanBan_ngang( tep, "Đặc trưng cho những nguyên tố số nguyên tử lớn hơn 94 ưa là dự đoán từ lý thuyết vì chỉ được sản xuất một số nguyên tử rất nhỏ mà phân rã rất nhanh.", gocX, gocY, "Arial", 30.0f, "Normal",
                kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
   
   // áp suất cho heli chảy
   gocX = kLE_TRAI + (2.0f + 14.0f + 10.0f + 4.9f)*(coO_ngang + giuaNgang);
   gocY = kLE_TREN + 0.62f*coO_doc;
   vanBan_ngang( tep, "P = 2,5 MPa", gocX, gocY, "Arial", 14.0f, "Normal", kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "end" );
   float x1 = gocX + 0.02f*coO_ngang;
   float y1 = gocY - 0.03f*coO_doc;
   float x2 = gocX + 0.2f*coO_ngang;
   duong( tep, x1, y1, x2, y1, 2.0f, 0x000000, 1.0f );
   
   // ==== mô tả đặc trưng
   NguyenTo *nguyenToViDu = &(mangNguyenTo[MANGAN]);
   float phongTo = 1.50f;
   gocX = kLE_TRAI + coO_ngang*5.0f;
   gocY = kLE_TREN + coO_doc*2.0f;

   soDoMoTaDacTrung( tep, nguyenToViDu, gocX, gocY, coO_ngang*phongTo, coO_doc*phongTo, phongTo );
   
   // ==== tên công ty
   gocX = kLE_TRAI - 0.2f*coO_ngang;
   gocY = kLE_TREN + 7.6f*(coO_doc + giuaDoc);
      vanBan_ngang( tep, "THNN TINH PHỤNG 2023.06.13 - An Giang, Phạm Vi công cộng", gocX, gocY, "Arial", 25.0f, "Normal", kDUNG, 0x00000, 1.0f, kSAI, 0.0f, 0x00, 0.0f, "start" );
};

#pragma mark ==== main

int main( int argc, char *argv[] ) {

   if( argc == 1 ) {
      FILE *tapTinSVG = fopen( "BảngTuầnHoàn.svg", "w" );

      if( tapTinSVG ) {
         NguyenTo *mangNguyenTo = malloc( kSO_LUONG__NGUYEN_TO*sizeof( NguyenTo ) );
         
         if( mangNguyenTo == NULL ) {
            printf( "Có vấn đề giàng bộ nhớ.\n" );
            exit(1);
         }

         // ---- Chuẩn bị thông tin nguyên tố
         chuanBiThongTinNguyenTo( mangNguyenTo );
         
         // ---- tính cỡ thước
         unsigned char soLuongO_ngang = 32;
         unsigned char soLuongO_doc = 7;
         float coO_ngang = kBE_RONG__KHO - kLE_TRAI - kLE_PHAI - (soLuongO_ngang-1)*kGIUA_NGANG;
         coO_ngang /= (float)soLuongO_ngang;
         
         float coO_doc = kBE_CAO__KHO - kLE_DUOI - kLE_TREN - (soLuongO_doc-1)*kGIUA_DOC;
         coO_doc /= (float)soLuongO_doc;
         
         // ==== SVG
         // ----ghi đầu tập tin SVG
         ghiDauSVG( tapTinSVG, kBE_RONG__KHO, kBE_CAO__KHO );
 
         printf( "Khổ ô: %5.1f  %5.1f\n", coO_ngang, coO_doc );
         veBangTuanHoanDungCoO( tapTinSVG, coO_ngang, coO_doc, mangNguyenTo, kLE_TRAI, kLE_TREN,
                               kGIUA_NGANG, kGIUA_DOC );

         // ---- kết thúc tệp SVG
         fprintf( tapTinSVG, "</svg>\n" );
         
         fclose( tapTinSVG );
         
         free( mangNguyenTo );
      }
      else {
         printf( "Có vấn đề mở tập tin SVG.\n");
         exit(1);
      }

      // ---- kết thúc tệp SVG
      fprintf( tapTinSVG, "</svg>\n" );

      fclose( tapTinSVG );
   }
   else {
      printf( "Cách sử dụng:\n  5: ./>\n\n" );
   }
   return 0;
}
