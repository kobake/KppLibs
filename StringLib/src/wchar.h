#pragma once



//�ėp�ϊ�
int wctoi_radix(int c,int radix);
int itowc_radix(int i,int radix);

//����}�N��
inline bool wc_is_num10(int c){ return wctoi_radix(c,10)!=-1; }
inline bool wc_is_num8 (int c){ return wctoi_radix(c, 8)!=-1; }
inline bool wc_is_num16(int c){ return wctoi_radix(c,16)!=-1; }

//�ϊ��}�N��
inline int wctoi10(int c){ return wctoi_radix(c,10); }
inline int wctoi8 (int c){ return wctoi_radix(c, 8); }
inline int wctoi16(int c){ return wctoi_radix(c,16); }

