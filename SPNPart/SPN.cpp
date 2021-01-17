/* SPN */
#include <cstdio>

using namespace std;

const int Nr = 4;	//����
//const int l = 4, m = 4;	//m��l����s��
const unsigned short SubstiBox[16] = {	//S��
	0xe,0x4,0xd,0x1,
	0x2,0xf,0xb,0x8,
	0x3,0xa,0x6,0xc,
	0x5,0x9,0x0,0x7
};
const unsigned short InverSubstiBox[16] = {		//S����
	0xe,0x3,0x4,0x8,
	0x1,0xc,0xa,0xf,
	0x7,0xd,0x9,0x6,
	0xb,0x2,0x0,0x5
};

unsigned short SPN(unsigned short x,const unsigned short* k,
	const unsigned short* sBox);
unsigned short substitute(unsigned short u, const unsigned short* sBox);
unsigned short permutate(unsigned short v);
void arrangeEncKey(unsigned key, unsigned short* k);
void adjustDecKey(unsigned short* k);

// ���
template<typename T>
inline void read(T & res) {
	char ch = getchar();
	res = 0;
	while ((ch < '0' || ch>'9') && (ch < 'a' || ch>'f'))
		ch = getchar();
	char num;
	while (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f') {
		num = (ch >= '0' && ch <= '9') ? ch - '0' : ch - 'a' + 10;
		res = (res << 4) + num, ch = getchar();
	}
}

// ��д
inline void print(unsigned short n)
{
	unsigned short mask = 0xf;
	for (int i = 0; i < 4; ++i) {
		char tmp = (n >> (3 - i)*4) & mask;
		tmp < 10 ? putchar(tmp + '0') : putchar(tmp - 10 + 'a');
	}
}


int main() {
	int n;
	scanf("%d", &n);
	unsigned key;
	unsigned short x;
	unsigned short k[Nr + 1];
	for (int i = 0; i < n; ++i) {
		read(key), read(x);
		arrangeEncKey(key, k);		//��Կ����
		unsigned short y = SPN(x,k,SubstiBox);
		print(y), putchar(' ');

		y = y ^ 0x1;
		adjustDecKey(k);	//������ԿΪ������Կ
		print(SPN(y,k,InverSubstiBox)), putchar('\n');
	}
	return 0;
}


// ��Կ����
inline void arrangeEncKey(unsigned key, unsigned short* k) {
	//unsigned mask = 0xffffffff >> (2 * l);
	//for (int i = 0; i < Nr + 1; ++i) {
	//	k[i] = (key >> ((m - i) * l)) & mask;
	//}
	k[0] = (key >> 16) & 0xffff, k[1] = (key >> 12) & 0xffff,
		k[2] = (key >> 8) & 0xffff, k[3] = (key >> 4) & 0xffff,
		k[4] = key & 0xffff;
}

// ��Կ����
void adjustDecKey(unsigned short* k) {
	unsigned short tmp;
	for (int i = 0; i < (Nr + 1) / 2; ++i) {
		tmp = k[i], k[i] = k[Nr - i], k[Nr - i] = tmp;
	}
	for (int i = 1; i < Nr; i++) {
		k[i] = permutate(k[i]);
	}
}


unsigned short SPN(unsigned short x, const unsigned short* k,
	const unsigned short* sBox) {
	unsigned short w = x, u, v;
	for (int i = 0; i < Nr - 1; ++i) {
		u = k[i] ^ w;	//�׻�
		v = substitute(u,sBox);		//����
		w = permutate(v);	//�û�
	}
	//���һ��
	u = k[Nr - 1] ^ w;
	v = substitute(u,sBox);
	return k[Nr] ^ v;
}

//����
inline unsigned short substitute(unsigned short u, const unsigned short* sBox) {
	//unsigned short res = 0, mask = 0xf;
	//for (int i = 0; i < m; ++i) {
	//	res |= sBox[(u >> (i * l) & mask)] << (i * l);
	//}
	//return res;
	unsigned short res = sBox[u & 0xf] | sBox[(u >> 4) & 0xf] << 4
		| sBox[(u >> 8) & 0xf] << 8 | sBox[(u >> 12) & 0xf] << 12;
	return res;
}

//�û�
inline unsigned short permutate(unsigned short v) {
	//unsigned short res = 0;
	//int len = l * m;
	//unsigned short tmp;
	//for (int i = 1; i <= len; ++i) {
	//	tmp = v >> (len - pBox[i]);
	//	res |= (tmp & 0x1) << (len - i);
	//}
	//return res;
	//p�жԳ���p�е���==p��, ��˿���ֱ�Ӵ��
	unsigned short res = ((v >> 15) & 1) << 15 | ((v >> 11) & 1) << 14
		| ((v >> 7) & 1) << 13 | ((v >> 3) & 1) << 12 | ((v >> 14) & 1) << 11
		| ((v >> 10) & 1) << 10 | ((v >> 6) & 1) << 9 | ((v >> 2) & 1) << 8
		| ((v >> 13) & 1) << 7 | ((v >> 9) & 1) << 6 | ((v >> 5) & 1) << 5
		| ((v >> 1) & 1 )<< 4 | ((v >> 12) & 1) << 3 | ((v >> 8) & 1) << 2
		| ((v >> 4) & 1) << 1 | (v & 1);
	return res;
}

/**/