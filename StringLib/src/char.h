#pragma once


//�ėp�ϊ�
int ctoi_radix(int c,int radix);
int itoc_radix(int i,int radix);

//����}�N��
inline bool is_num10(int c){ return ctoi_radix(c,10)!=-1; }
inline bool is_num8 (int c){ return ctoi_radix(c, 8)!=-1; }
inline bool is_num16(int c){ return ctoi_radix(c,16)!=-1; }

//�ϊ��}�N��
inline int ctoi10(int c){ return ctoi_radix(c,10); }
inline int ctoi8 (int c){ return ctoi_radix(c, 8); }
inline int ctoi16(int c){ return ctoi_radix(c,16); }

