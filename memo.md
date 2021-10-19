9/19

やったこと
* FrameBuffer screen;で定義されているscreenの役割について読んでいた.
次回
* screen軽く確認したら、本読みながら9cを追っていく.終わったら次へ.

10/14
* APICやACPIについて調べてた.
  * 大神さんの神ブログも見つけたので、今後取り組みたい.
  * かなりハードに近い部分なので、根気がいる
  * 自作本を読むときも、どこまでがハード由来でどこまでがc++による実現かの区別をつけると良さそう.


### C++ specific syntax.
* scope resolution operator.

```
int count = 0;

int main(void) {
   int count = 0;
   ::count = 1;  // set global count to 1
   count = 2;    // set local count to 2
   return 0;
}
```

* C++ Inheritance

```
class Animal{

}

class Cat : Animal{

}
```

* C++ using keyword (constructor inheritence.).

* no-name namespace
ファイル外からはアクセスできない変数や関数をおくことができる.

### 参考記事

#### 11章
* linuxカーネルにおけるtimerについて
  * https://cstmize.hatenablog.jp/entry/2019/03/29/linux_kernel%E3%81%AB%E3%81%8A%E3%81%91%E3%82%8Btimer%E3%81%AB%E3%81%A4%E3%81%84%E3%81%A6
* APIC timer 
  * https://wiki.osdev.org/APIC_timer
    * [PIT](https://wiki.osdev.org/PIT)とよく比較される
    * mikan osではこのtimerを使用している.
    * PITと比較した際の特徴は
      * PCに直接回路が埋め込まれているので、すぐに使用できる.
      * 周波数がマシンによって一定出ない.ので、secondなどに変換する際は別途変換pathを噛ませる必要がある.
* APIC
  * https://wiki.osdev.org/APIC
    * advancedなPIC.
* PIC
  * [8259 PIC](https://wiki.osdev.org/PIC)
    * hardwareは発生させた割り込みを適切にカーネルに通知する部分を担当する.
* EFI system table
  * 参考
    * https://edk2-docs.gitbook.io/edk-ii-uefi-driver-writer-s-guide/3_foundation/33_uefi_system_table
    * https://sites.google.com/site/uefiforth/bios/uefi/uefi/4-efi-system-table/4-3-efi-system-**table** 
* ACPI
  * [ACPI](https://ja.wikipedia.org/wiki/Advanced_Configuration_and_Power_Interface)
  * [わかりそうで](https://wa3.i-3-i.info/word14319.html)
* RSDP
  * [RSDP](https://wiki.osdev.org/RSDP)