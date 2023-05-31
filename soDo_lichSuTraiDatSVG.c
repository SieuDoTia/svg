// soDo_LichSuTraiDat_SVG.c
// Sơ đồ lịch sử Trái Đất/lịch sử địa chất
// Phạm vi công cộng

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---- khổ chuẩn cho nửa A0 ngang
#define kBE_RONG__KHO 4494
#define kBE_CAO__KHO  1589

#define kLE_TRAI 150.0f  // cho số trục y
#define kLE_PHAI 150.0f
#define kLE_DUOI 130.0f  // cho số trục x
#define kLE_TREN 300.0f  // cho tiêu đề

#define kSAI  0
#define kDUNG 1

/*
+-------------------------------------+
|          9500 triệu năm             |
| +--------------+------------------+ |
| +--------------+------------------+ |
|                                     |
|           541 triệu năm             |
| +---------------------------------+
| +---------------------------------+
*/

// ---- toàn lịch sử Trái Đất
/*

Liên Đại Hỏa Thành  4500 - 3800
Liên Đại Thái Cổ 3800 - 2500
Liên Đại Nguyên Sinh 2500 - 542
  Đại Cổ Nguyên  Sinh 2500 - 1600
  Đại Trung Nguyên Sinh 1600 - 1000
  Đại Tân Nguyên Sinh 1000 -

Liên Đại Hiển Sinh 541 - 0
  Đại Cổ Sinh 541 - 252
  Đại Trung Sinh 252 - 66
  Đại Tân Sinh 66 - 0

Tương Lại 0 5000
*/


/*
Liên Đại Hiển Sinh 541 - 0
Đại Cổ Sinh
  Cambri 541 ± 1 - 485 ± 1.9
  Ordovic 485 ± 1.9 - 444 ± 1.5
  Silur 444 ± 1.5 - 416,2 ± 2.8
  Devon 416,2 ± 2.8 - 359.2 ± 2.5
  Than Đá 359.2 ± 2.5 - 299.0 ± 0.8
  Permi 299.0 ± 0.8 - 252.2 ± 0.06

Đại Trung Sinh
  Tam Điệp 252.2 ± 0.06 - 199.3 ± 0.6
  Jura          199.3 - 145
  Phấn Trắng  145.0 - 65.8 ± 0.3

Đại Tân Sinh
  Paleogen. 65.5 ± 0.3 - 23.03 ± 0.05
  Neogen 23.03 - 2.58
  Đệ Tứ 2.58 - 0.00
*/

typedef struct {
   short batDau;
   short ketThuc;
   unsigned int mau;
   char ten[64];
} Dai;

enum {LIEN_DAI__HINH_THANH,
   LIEN_DAI__HOA_THANH,
   LIEN_DAI__THAI_CO,
   LIEN_DAI__NGUYEN_SINH,
   LIEN_DAI__HIEN_SINH,
   LIEN_DAI__TUONG_LAI
};

enum {DAI__CO_NGUYEN_SINH,
   DAI__TRUNG_NGUYEN_SINH,
   DAI__TAN_NGUYEN_SINH,
   DAI__CO_SINH,
   DAI__TRUNG_SINH,
   DAI__TAN_SINH,
};

enum {KY__CAMBRI,
   KY__ORDOVIC,
   KY__SILUR,
   KY__DEVON,
   KY__THAN_DA,
   KY__PERMI,
   KY__TAM_DIEP,
   KY__JURA,
   KY__PHAN_TRANG,
   KY__PALEOGEN,
   KY__NEOGEN,
   KY__DE_TU,
};


#pragma mark ==== SVG
void luuDauSVG( FILE *tep, unsigned int beRong, unsigned int beCao ) {    // ---- đầu tập tin SVG
   
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

#pragma mark ==== Chuẩn Bị
unsigned char chuanBiLienDai( Dai **danhSachLienDai ) {
   
   *danhSachLienDai = malloc( (LIEN_DAI__TUONG_LAI + 1)*sizeof( Dai ));

   if( danhSachLienDai == NULL ) {
       printf( "Vấn đề giàng bộ nhớ\n" );
       exit(1);
   }

   Dai *lienDai = *danhSachLienDai;
   
   // ---- Hình Thanh Trái Đất
   strcpy( lienDai[LIEN_DAI__HINH_THANH].ten, "Hình Thanh Trái Đất" );
   lienDai[LIEN_DAI__HINH_THANH].batDau = -5000;
   lienDai[LIEN_DAI__HINH_THANH].ketThuc = -4500;
   lienDai[LIEN_DAI__HINH_THANH].mau = 0x000000;

   // ---- Liên Đại Hỏa Thành
   strcpy( lienDai[LIEN_DAI__HOA_THANH].ten, "Liên Đại Hỏa Thành" );
   lienDai[LIEN_DAI__HOA_THANH].batDau = -4500;
   lienDai[LIEN_DAI__HOA_THANH].ketThuc = -3800;
   lienDai[LIEN_DAI__HOA_THANH].mau = 0x00ffff;

   // ---- Liên Đại Thái Cố
   strcpy( lienDai[LIEN_DAI__THAI_CO].ten, "Liên Đại Thái Cố" );
   lienDai[LIEN_DAI__THAI_CO].batDau = -3800;
   lienDai[LIEN_DAI__THAI_CO].ketThuc = -2500;
   lienDai[LIEN_DAI__THAI_CO].mau = 0x007fff;

   // ---- Liên Đại Nguyên Sinh
   strcpy( lienDai[LIEN_DAI__NGUYEN_SINH].ten, "Liên Đại Nguyên Sinh" );
   lienDai[LIEN_DAI__NGUYEN_SINH].batDau = -2500;
   lienDai[LIEN_DAI__NGUYEN_SINH].ketThuc = -541;
   lienDai[LIEN_DAI__NGUYEN_SINH].mau = 0x0000ff;
   
   // ---- Liên Đại Hiển Sinh
   strcpy( lienDai[LIEN_DAI__HIEN_SINH].ten, "Liên Đại Hiển Sinh" );
   lienDai[LIEN_DAI__HIEN_SINH].batDau = -541;
   lienDai[LIEN_DAI__HIEN_SINH].ketThuc = 0;
   lienDai[LIEN_DAI__HIEN_SINH].mau = 0x0000ff;
   
   // ---- Liên Đại Tương Lai
   strcpy( lienDai[LIEN_DAI__TUONG_LAI].ten, "Liên Đại Tương Lai" );
   lienDai[LIEN_DAI__TUONG_LAI].batDau = 0;
   lienDai[LIEN_DAI__TUONG_LAI].ketThuc = 5000;
   lienDai[LIEN_DAI__TUONG_LAI].mau = 0x7f005f;

   return LIEN_DAI__TUONG_LAI + 1;
}

unsigned char chuanBiDai( Dai **danhSachDai ) {

   *danhSachDai = malloc( (DAI__TAN_SINH + 1)*sizeof( Dai ));
   
   if( danhSachDai == NULL ) {
      printf( "Vấn đề giàng bộ nhớ\n" );
      exit(1);
   }

   Dai *dai = *danhSachDai;
   
   // ---- Đại Cổ Ngyuên

   strcpy( dai[DAI__CO_NGUYEN_SINH].ten, "Đại Cổ Nguyên Sinh" );
   dai[DAI__CO_NGUYEN_SINH].batDau = -2500;
   dai[DAI__CO_NGUYEN_SINH].ketThuc = -1600;
   dai[DAI__CO_NGUYEN_SINH].mau = 0x7f5f10;

   // ---- Đại Trung Ngyuên
   strcpy( dai[DAI__TRUNG_NGUYEN_SINH].ten, "Đại Trung Nguyên Sinh" );
   dai[DAI__TRUNG_NGUYEN_SINH].batDau = -1600;
   dai[DAI__TRUNG_NGUYEN_SINH].ketThuc = -1000;
   dai[DAI__TRUNG_NGUYEN_SINH].mau = 0xff5f10;
   
   // ---- Đại Tân Ngyuên
   strcpy( dai[DAI__TAN_NGUYEN_SINH].ten, "Đại Tân Nguyên Sinh" );
   dai[DAI__TAN_NGUYEN_SINH].batDau = -1000;
   dai[DAI__TAN_NGUYEN_SINH].ketThuc = -541;
   dai[DAI__TAN_NGUYEN_SINH].mau = 0xffaf10;
   
   // ---- Đại Cổ Sinh
   strcpy( dai[DAI__CO_SINH].ten, "Đại Cổ Sinh" );
   dai[DAI__CO_SINH].batDau = -541;
   dai[DAI__CO_SINH].ketThuc = -252;
   dai[DAI__CO_SINH].mau = 0x5fff4f;
   
   // ---- Đại Trung Sinh
   strcpy( dai[DAI__TRUNG_SINH].ten, "Đại Trung Sinh" );
   dai[DAI__TRUNG_SINH].batDau = -252;
   dai[DAI__TRUNG_SINH].ketThuc = -66;
   dai[DAI__TRUNG_SINH].mau = 0x007fff;

   // ---- Đại Tân Sinh
   strcpy( dai[DAI__TAN_SINH].ten, "Đại Tân Sinh" );
   dai[DAI__TAN_SINH].batDau = -66;
   dai[DAI__TAN_SINH].ketThuc = -0;
   dai[DAI__TAN_SINH].mau = 0x00ffff;

   return DAI__TAN_SINH + 1;
}

unsigned char chuanBiKy( Dai **danhSachKy ) {

   *danhSachKy = malloc( (KY__DE_TU + 1)*sizeof( Dai ));
   
   if( danhSachKy == NULL ) {
      printf( "Vấn đề giàng bộ nhớ\n" );
      exit(1);
   }
   
   Dai *ky = *danhSachKy;
   
   // ---- Kỳ Cambri
   strcpy( ky[KY__CAMBRI].ten, "Kỳ Cambri" );
   ky[KY__CAMBRI].batDau = -541;
   ky[KY__CAMBRI].ketThuc = -485;
   ky[KY__CAMBRI].mau = 0x0f0f0f;
   
   // ---- Kỳ Ordovic
   strcpy( ky[KY__ORDOVIC].ten, "Kỳ Ordovic" );
   ky[KY__ORDOVIC].batDau = -485;
   ky[KY__ORDOVIC].ketThuc = -444;
   ky[KY__ORDOVIC].mau = 0x2f2f2f;
   
   // ---- Kỳ Silur
   strcpy( ky[KY__SILUR].ten, "Kỳ Silur" );
   ky[KY__SILUR].batDau = -444;
   ky[KY__SILUR].ketThuc = -416;
   ky[KY__SILUR].mau = 0x4f4f4f;

   // ---- Kỳ Devon
   strcpy( ky[KY__DEVON].ten, "Kỳ Devon" );
   ky[KY__DEVON].batDau = -416;
   ky[KY__DEVON].ketThuc = -359;
   ky[KY__DEVON].mau = 0x6f6f6f;

   // ---- Kỳ Than Đá
   strcpy( ky[KY__THAN_DA].ten, "Kỳ Than Đá" );
   ky[KY__THAN_DA].batDau = -359;
   ky[KY__THAN_DA].ketThuc = -299;
   ky[KY__THAN_DA].mau = 0xffffff;
   
   // ---- Kỳ Permi
   strcpy( ky[KY__PERMI].ten, "Kỳ Permi" );
   ky[KY__PERMI].batDau = -299;
   ky[KY__PERMI].ketThuc = -252;
   ky[KY__PERMI].mau = 0x007fff;
   
   // ---- Kỳ Tam Điệp
   strcpy( ky[KY__TAM_DIEP].ten, "Kỳ Tam Điệpi" );
   ky[KY__TAM_DIEP].batDau = -252;
   ky[KY__TAM_DIEP].ketThuc = -199;
   ky[KY__TAM_DIEP].mau = 0x58865a;

   // ---- Kỳ Jura

   strcpy( ky[KY__JURA].ten, "Kỳ Jura" );
   ky[KY__JURA].batDau = -199;
   ky[KY__JURA].ketThuc = -145;
   ky[KY__JURA].mau = 0xd18a9c;
   
   // ---- Kỳ Phấn Trắng

   strcpy( ky[KY__PHAN_TRANG].ten, "Kỳ Phấn Trắng" );
   ky[KY__PHAN_TRANG].batDau = -145;
   ky[KY__PHAN_TRANG].ketThuc = -66;
   ky[KY__PHAN_TRANG].mau = 0xf9f0d4;
   
   // ---- Kỳ Paleogen
   strcpy( ky[KY__PALEOGEN].ten, "Kỳ Paleogen" );
   ky[KY__PALEOGEN].batDau = -66;
   ky[KY__PALEOGEN].ketThuc = -23;
   ky[KY__PALEOGEN].mau = 0xc3f0d4;
                     
   // ---- Kỳ Neogen
   strcpy( ky[KY__NEOGEN].ten, "Kỳ Neogen" );
   ky[KY__NEOGEN].batDau = -23;
   ky[KY__NEOGEN].ketThuc = -3;
   ky[KY__NEOGEN].mau = 0xfbf023;
   
   // ---- Kỳ Đệ Tứ
   strcpy( ky[KY__DE_TU].ten, "Kỳ Đệ Tứ" );
   ky[KY__DE_TU].batDau = -3;
   ky[KY__DE_TU].ketThuc = 0;
   ky[KY__DE_TU].mau = 0xfbf0c;
                          
   return KY__DE_TU + 1;
}

#pragma mark ==== vẽ
void veSoDoDai( FILE *tep, Dai *danhSachDai, unsigned char soLuongDai, float phongToNgang, float beCaoSoDo, float dichX, float dichY, unsigned char soTren ) {
   
   float coThuocPhong_vanBan = 25.0f;
   float coThuocPhong_so = 50.0f;
   
   // vẽ từ trái sang phải tương đối với điểm này
   float thoiGianTuongDoi = danhSachDai[0].batDau;   // cho cảnh phía trái sơ đồ
   float gocY_chuNhat = dichY;
   float gocY_vanBanTen = dichY + beCaoSoDo * 0.5f;
   
   float gocY_vanBanSo = 0.0f;
   if( soTren )
      gocY_vanBanSo = dichY - coThuocPhong_so*0.75f;
   else
      gocY_vanBanSo = dichY + beCaoSoDo + coThuocPhong_so * 1.3f;

   float thoiGianBatDau = 0.0f;
   float thoiGianKetThuc = 0.0f;

   unsigned char soDai = 0;
   while( soDai < soLuongDai) {
      // ---- tính chữ nhật
      thoiGianBatDau = danhSachDai[soDai].batDau;
      thoiGianKetThuc = danhSachDai[soDai].ketThuc;
      unsigned int mauChuNhat = danhSachDai[soDai].mau;

      float gocX_chuNhat = (thoiGianBatDau - thoiGianTuongDoi)*phongToNgang + dichX;
      float beRongChuNhat = (thoiGianKetThuc - thoiGianBatDau)*phongToNgang;
      chuNhat( tep, gocX_chuNhat, gocY_chuNhat, beRongChuNhat, beCaoSoDo, 1, mauChuNhat, 1.0f,
           1, 2.0, 0xffffff, 0.5f );
      
      // ---- văn bản - tên
      float gocX_vanBan = beRongChuNhat*0.5 + gocX_chuNhat;
      char *tenDai = danhSachDai[soDai].ten;
      vanBan_ngang( tep, tenDai, gocX_vanBan, gocY_vanBanTen, "Arial", coThuocPhong_vanBan, "Normal",
                        1, 0x000000, 1.0f, 0, 0.0f, 0x000000, 0.0f, "middle" );

      // ---- văn bản - số
      char vanBanSo[32];
      sprintf( vanBanSo, "%5.0f", thoiGianBatDau );
      vanBan_ngang( tep, vanBanSo, gocX_chuNhat, gocY_vanBanSo, "Arial", coThuocPhong_so, "Normal",
                   1, 0x000000, 1.0f, 0, 0.0f, 0x000000, 0.0f, "middle" );
      
      soDai++;
   }

   float gocX_vanBanSo = (thoiGianKetThuc - thoiGianTuongDoi)*phongToNgang + dichX;
   char vanBanSo[32];
   sprintf( vanBanSo, "%5.0f", thoiGianKetThuc );
   vanBan_ngang( tep, vanBanSo, gocX_vanBanSo, gocY_vanBanSo, "Arial", coThuocPhong_so, "Normal",
                1, 0x000000, 1.0f, 0, 0.0f, 0x000000, 0.0f, "middle" );

}

#pragma mark ==== main

int main( int argc, char *argv[] ) {

   if( argc == 1 ) {
      FILE *tapTinSVG = fopen( "LịchSửTráiĐất.svg", "w" );

      if( tapTinSVG ) {

         // ---- lưu đầu tập tin SVG
         luuDauSVG( tapTinSVG, kBE_RONG__KHO, kBE_CAO__KHO );
         
         // ====
         char *tieuDe = "Lịch Sử Địa Chất Trái Đất";
         vanBan_ngang( tapTinSVG, tieuDe, kBE_RONG__KHO >> 1, kLE_TREN*0.6f, "Arial", 100.0f, "Bold",
                      1, 0x000000, 1.0f, 0, 0.0f, 0x000000, 0.0f, "middle" );
         
         // ==== liên đại
         Dai *danhSachLienDai = NULL;
         unsigned char soLuongLienDai = chuanBiLienDai( &danhSachLienDai );
         
         // ---- tính phóng to
         float beCaoChoLienDai = 80.0f;
         float thoiGian = danhSachLienDai[LIEN_DAI__TUONG_LAI].ketThuc - danhSachLienDai[LIEN_DAI__HINH_THANH].batDau;
         
         float beRongNgang = kBE_RONG__KHO - kLE_TRAI - kLE_PHAI;
         float phongToNgangLienDai = beRongNgang/thoiGian;

         // ---- vẽ
         veSoDoDai( tapTinSVG, danhSachLienDai, LIEN_DAI__TUONG_LAI+1, phongToNgangLienDai, beCaoChoLienDai, kLE_TRAI, kLE_TREN, kDUNG );

         // ==== kỳ của liên đại hiển sinh (hiện đại)
         Dai *danhSachKy = NULL;
         unsigned char soLuongKy = chuanBiKy( &danhSachKy );
         
         // ---- tính phóng to
         float beCaoChoKy = 500.0f;
         thoiGian = danhSachKy[KY__DE_TU].ketThuc - danhSachKy[KY__CAMBRI].batDau;
         float phongToNgangKy = beRongNgang/thoiGian;

         // ---- vẽ
         veSoDoDai( tapTinSVG, danhSachKy, KY__DE_TU+1, phongToNgangKy, beCaoChoKy, kLE_TRAI, kLE_TREN + 500, kSAI );
         
         // ==== đại
         Dai *danhSachDai = NULL;
         unsigned char soLuongDai = chuanBiDai( &danhSachDai );
         
         // ----- vẽ bắt đầu từ Liên Đại Nguyên Sinh
         float gocX = kLE_TRAI + (danhSachLienDai[LIEN_DAI__NGUYEN_SINH].batDau - danhSachLienDai[LIEN_DAI__HINH_THANH].batDau)*phongToNgangLienDai;
         float gocY = kLE_TREN + beCaoChoLienDai;
         veSoDoDai( tapTinSVG, danhSachDai, DAI__TAN_SINH+1, phongToNgangLienDai, beCaoChoLienDai, gocX, gocY, kSAI );

         // ----- vẽ cho dãy kỳ, từ đại cổ sinh
         gocY = kLE_TREN + 500 - beCaoChoLienDai;
         veSoDoDai( tapTinSVG, &(danhSachDai[DAI__CO_SINH]), DAI__TAN_SINH+1 - DAI__TAN_NGUYEN_SINH, phongToNgangKy, beCaoChoLienDai, kLE_TRAI, gocY, kDUNG );
         
         free( danhSachLienDai );
         free( danhSachDai );
         free( danhSachKy );
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

