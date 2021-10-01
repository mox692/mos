9/19

やったこと
* FrameBuffer screen;で定義されているscreenの役割について読んでいた.
次回
* screen軽く確認したら、本読みながら9cを追っていく.終わったら次へ.



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