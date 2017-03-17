# Arduino STM32 内部フラッシュメモリ書き込みライブラリ
## 概要
Arduino STN32環境にてSTM32マイコンの内部フラッシュメモリへのデータ書込みを行うライブラリです.  
本ライブラリのAPIを使ってスケッチ(プログラム)から内フラッシュメモリの書き換えが可能です.  

## 利用環境
- 開発環境 Arduino IDE に Arduino STM32をインストールしていること.
- STM32マイコン: Arduino STM32にて開発が可能であること.

## ライブラリ名称
**TFlash** (ヘッダーファイル TFlash.h)

## 特徴・制約等
- 現時点ではSTM32F103C8T6のみ動作確認を行っています.  
- 利用可能フラッシュメモリ領域 0x8002000 ～ 0x0807FFFF のうちでメモリ実装領域
- Arduino STM32用ブートローダー領域 0x8000000 ～ 0x8002000への書込みは禁止しています.  
- フラッシュメモリ仕様により、ページ単位の消去、16ビット(2バイト)単位の書込みを行うことが出来ます.  

## インストール
TFlashフォルダを各自のArduino STM32インストール先のライブラリ用フォルダに配置する.  
=> インストールフォルダ\hardware\Arduino_STM32\STM32F1\libraries\  

## ライブラリリファレンス
ライブラリはクラスライブラリとして実装しています.  
フラッシュメモリ操作はグローバルオブジェクト**TFlash**を利用して行います. 
### ヘッダーファイル
`#include <TFlash.h>`  

### グローバルオブジェクト
`TFlash`  

### 定数
#### フラッシュメモリ操作実行結果  
ライブラリ関数はフラッシュメモリ操作の結果としてと次の**TFLASH_Status**型の値を返します.  
```
typedef enum  {
  TFLASH_BUSY = 1,    // 処理中
  TFLASH_ERROR_PG,    // 書込み失敗
  TFLASH_ERROR_WRP,   // 書込み保護による書き込み失敗
  TFLASH_ERROR_OPT,   // 操作異常
  TFLASH_COMPLETE,    // 書込み完了
  TFLASH_TIMEOUT,     // 書込み完了待ちタイムアウト
  TFLASH_BAD_ADDRESS  // 不当操作
} TFLASH_Status;
```
実際の利用においては、**TFLASH_COMPLETE**が返された場合は**正常終了**、  
それ以外は**異常終了**として処理して下さい.  

### パブリックメンバー関数
#### 指定ページ消去

- 書式  
 `TFLASH_Status eracePage(uint32_t pageAddress)`  

- 引数  
 pageAddress :フラッシュメモリアドレス    
 
- 戻り値  
 正常終了: TFLASH_COMPLETE  
 異常終了: 上記以外の値

- 説明  
 フラッシュメモリの指定領域をページ単位で消去します.  
 pageAddressにはページの先頭アドレスを指定します.   
 
- 注意  
  書込み済みのアドレスに対して書き込みを行う場合は、事前にページ単位の消去が必要となります.  
  消去・書き込前にlock()関数にてロック解除をして下さい.  
  消去・書き込後はunlock()関数にてロックかけて下さい.  
  ページ数および、ページ内バイト数はSTM32マイコンの種類により異なります.  
  詳細については、ご利用マイコンのデータシート、リファレンスマニュアル等にて確認ください.  

#### 2バイトデータ書込み

- 書式  
 `TFLASH_Status write(uint16_t* adr, uint16_t data)`  

- 引数  
 adr : 書込み先アドレス  
 data: 書込みデータ   
 
- 戻り値  
 正常終了: TFLASH_COMPLETE  
 異常終了: 上記以外の値

- 説明  
 指定アドレスに16ビット(2バイト)データを書きこみます.  
 
- 注意  
  書込み済みのアドレスに対して書き込みを行う場合は、事前にページ単位の消去が必要となります.  
  消去・書き込前にlock()関数にてロック解除をして下さい.  
  消去・書き込後はunlock()関数にてロックかけて下さい.  
  ページ数および、ページ内バイト数はSTM32マイコンの種類により異なります.  
  詳細については、ご利用マイコンのデータシート、リファレンスマニュアル等にて確認ください.  

#### バイト列データ書込み

- 書式  
`TFLASH_Status write(uint16_t* adr, uint8_t* data, uint16_t len)`  

- 引数  
 adr : 書込み先アドレス  
 data: 書込みバイト列データの先頭アドレス     
 len : 書込みデータバイト数  

- 戻り値    
 正常終了: TFLASH_COMPLETE  
 異常終了: 上記以外の値  

- 説明  
 指定アドレスにバイト列データを書きこみます.  
 フラッシュメモリへの書込みは16ビット単位のため、データ長が奇数バイトの場合は、  
 1バイト分0xFFを付加して書き込みを行います。  
- 注意  
  書込み済みのアドレスに対して書き込みを行う場合は、事前にページ単位の消去が必要となります.  
  消去・書き込前にlock()関数にてロック解除をして下さい.  
  消去・書き込後はunlock()関数にてロックかけて下さい.  
  ページ数および、ページ内バイト数はSTM32マイコンの種類により異なります.  
  詳細については、ご利用マイコンのデータシート、リファレンスマニュアル等にて確認ください.  

#### データ読込み

- 書式  
 `uint16_t read(uint16_t* adr)`  

- 引数  
 adr : 読込みアドレス  
 
- 戻り値  
 読み取ったデータ

- 説明  
 指定したアドレスに格納されているデータを返します.  
 フラッシュメモリ上のデータは直接アドレス指定にて参照することが可能ですので、  
 本関数の利用は不要です。他APIとの互換性を考慮して用意しています.  
 `uint16_t v = TFlash.read(adr)`は`uint16_t v = *adr`と等価です.

#### 書込み保護設定

- 書式  
 `void lock()`  

- 引数  
 なし  
 
- 戻り値  
 なし

- 説明  
 フラッシュメモリへの書込み保護を設定します.    

#### 書込み保護解除

- 書式  
 `void unlock()`  

- 引数  
 なし  
 
- 戻り値  
 なし

- 説明  
 フラッシュメモリへの書込み保護を解除します.    

## サンプルスケッチ
### stm32_testFlash
フラッシュメモリへの書込みを行い、その内容を表示するプログラムです.  
別途、mcursesライブラリ(https://github.com/ChrisMicro/mcurses)が必要です。  

```
#define FLASH_PAGE_SIZE        1024
#define FLASH_START_ADDRESS    ((uint32)(0x8000000))

#include <string.h>
#include "stm32_hexedit.h"
#include "TFlash.h"

uint8_t str1[] = "1234567890A";
uint8_t str2[] = "abcdefghij";

void Arduino_putchar(uint8_t c) {
  Serial.write(c);
}

char Arduino_getchar() {
  char c;
  while (!Serial.available());
  return Serial.read();
}

uint32_t adr0 = FLASH_START_ADDRESS + FLASH_PAGE_SIZE *  63;
uint32_t adr1 = FLASH_START_ADDRESS + FLASH_PAGE_SIZE *  127;

void setup() {
  Serial.begin(115200);
  while (!Serial.isConnected()) delay(100);
  setFunction_putchar(Arduino_putchar); 
  setFunction_getchar(Arduino_getchar); 
  initscr();

  // フラッシュメモリ書き込みテスト
  TFlash.unlock();
  TFlash.eracePage(adr0);
  TFlash.write((uint16_t*)adr0, str1, strlen((char*)str1));
  TFlash.eracePage(adr1);
  TFlash.write((uint16_t*)adr1, str2, strlen((char*)str2));
  TFlash.lock();
}

void loop() {
  // 64kバイトフラッシュメモリ最終ページの参照
  clear();
  hexedit2 (adr0, false);

  // 128kバイトフラッシュメモリ最終ページの参照
  clear();
  hexedit2 (adr1, false);  
}
```


