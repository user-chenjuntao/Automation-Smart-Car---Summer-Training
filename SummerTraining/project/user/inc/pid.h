#ifndef __ALGO_PID_H_
#define __ALGO_PID_H_

typedef struct
{
    float fCurr_Value;  /* ��ǰֵ */
    float fExp_Value;   /* �趨������ֵ */
	
    float fKp;          /* ����ϵ�� */
    float fKi;          /* ΢��ϵ�� */
    float fKd;          /* ����ϵ�� */
    
    float fPout;        /* ��������� */
    float fIout;        /* ��������� */
    float fDout;        /* ΢������� */

    float fCtrl_Out;    /* �������ֵ */
    float fPre_Out;     /* ǰһ�����ֵ */

    float fMax_Iout;    /* PID��������� */

    float fDbuf[3];     /* ΢�����������ֵ 0Ϊ���� 1��һ�� 2���ϴ� */
    float fError[3];    /* ������������ֵ 0Ϊ���� 1��һ�� 2���ϴ� */
	
    int fMax_Out;     /* PID������ */
	
}tagPID_T;

typedef struct
{
    float fKp;          /* ����ϵ�� */
    float fKi;          /* ΢��ϵ�� */
    float fKd;          /* ����ϵ�� */

    float fMax_Iout;    /* PID��������� */
    float fMax_Out;     /* PID������ */

}PIDInitStruct;

void PID_Init(tagPID_T *_tPid,PIDInitStruct *_tPidInit);
void PID_Update(tagPID_T *_tPid,PIDInitStruct *_tPidInit);
int PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue,float dt);
void PID_Clear(tagPID_T *_tPid);

#endif
