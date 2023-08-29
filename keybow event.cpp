// #include < conio.h > 
#include <iostream>
 
using namespace std ;
 
int main ( ) { int ch ;
    while ( 1 ) { if ( _kbhit ( ) ) { //如果有按鍵按下，則_kbhit()函數返回真      
        
            ch = _getch ( ) ; //使用_getch()函數獲取按下的鍵值
            cout << ch ;
            if ( ch == 27 ) { break ; } //當按下ESC時循環，ESC鍵的鍵值時27.  
        } } system ( " pause " ) ;
}