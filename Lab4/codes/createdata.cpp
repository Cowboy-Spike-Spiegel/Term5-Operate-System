#include<iostream>
#include<fstream>
#include<stdlib.h>
using namespace std;
int main()
{
	int dataNum = 20;//��������
	int timeMax=5;//��д���ʱ������100����Ϊ��λ
	char RW[2]={'R','W'};//�������ͣ���Ϊ����д����
	ofstream  data("data.txt");//�Ը��Ƿ�ʽ���ļ�
	if(data)//���ļ��ɹ�
	{
		srand((unsigned)time(NULL));
		for(int i=0;i<dataNum;i++){
			int index=rand()%2;//������д��ʽ
			int spendTime=(rand()%timeMax+1);//��д����ʱ��
			data<<RW[index]<<""<<spendTime;//������ļ�����
			if(i<dataNum-1)
				data<<"\r\n";
		}
		data.close();
	}
}
