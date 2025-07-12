#include "pid.h"

/**
 * @brief PID��ʼ����
 * @param _tPid-PID�ṹ��ָ��
 * @param _tPidInit-PID��ʼ���ṹ��
 * @retval Null
*/
void PID_Init(tagPID_T *_tPid,PIDInitStruct *_tPidInit)
{
	
	/* ��ʷ������0 */
    _tPid->fDbuf[0] = _tPid->fDbuf[1] = _tPid->fDbuf[2] = _tPid->fError[0] = _tPid->fError[1] = _tPid->fError[2] = _tPid->fPout = _tPid->fIout = _tPid->fDout = _tPid->fCtrl_Out = _tPid->fPre_Out = 0.0f;
	
	/* ������ֵ */
		_tPid->fKp = _tPidInit->fKp;
		_tPid->fKi = _tPidInit->fKi;
		_tPid->fKd = _tPidInit->fKd;
		
		_tPid->fMax_Iout = _tPidInit->fMax_Iout;
		_tPid->fMax_Out = _tPidInit->fMax_Out;
	
		_tPid->alpha = _tPidInit->alpha;

}

/**
 * @brief ����PIDϵ������
 * @param _tPid-PID�ṹ��ָ��
 * @param _tPidInit-PID��ʼ���ṹ��
 * @retval Null
*/
void PID_Update(tagPID_T *_tPid,PIDInitStruct *_tPidInit)
{
    /* ����PIDϵ�� */

		_tPid->fKp = _tPidInit->fKp;
		_tPid->fKi = _tPidInit->fKi;
		_tPid->fKd = _tPidInit->fKd;
		
		_tPid->fMax_Iout = _tPidInit->fMax_Iout;
		_tPid->fMax_Out = _tPidInit->fMax_Out;
	
		_tPid->alpha = _tPidInit->alpha;

}


///**
// * @brief PID���㺯��
// * @param _tPid-PID�ṹ��ָ��
// * @param _fCurrValue-��ǰֵ
// * @param _fExpValue-����ֵ
// * @param dt-ʱ��΢�֣��ӽ�����
// * @retval float ����PID���������ֵ
//*/
//int PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue,float dt)
//{
//    /* �趨����ֵ�͵�ǰֵ */
//    _tPid->fExp_Value  = _fExpValue;
//    _tPid->fCurr_Value = _fCurrValue;
//    
//	_tPid->fError[2] = _tPid->fError[1];
//	_tPid->fError[1] = _tPid->fError[0];
//    _tPid->fError[0] = _fExpValue - _fCurrValue;
//	
//    /* ����΢��������� */
//    _tPid->fDbuf[2] = _tPid->fDbuf[1];
//    _tPid->fDbuf[1] = _tPid->fDbuf[0];

//    /* ����΢����ƽ��*/
//    _tPid->fDbuf[0] = (_tPid->fError[0] - 2.0f * _tPid->fError[1] + _tPid->fError[2]) / 2.0f;
//	
//    _tPid->fPout = _tPid->fKp * _tPid->fError[0];
//    _tPid->fIout += _tPid->fKi * _tPid->fError[0] * dt;
//	_tPid->fDout = _tPid->fKd * _tPid->fDbuf[0] / dt;
//	//�޷�
//	if(_tPid->fIout > _tPid->fMax_Iout)
//		_tPid->fIout = _tPid->fMax_Iout;
//    
//    /* ������� */
//    _tPid->fPre_Out = _tPid->fCtrl_Out;
//    _tPid->fCtrl_Out = (int)(_tPid->fPout + _tPid->fIout + _tPid->fDout);
//	//�޷�
//	if(_tPid->fPre_Out > _tPid->fMax_Out)
//		_tPid->fPre_Out = _tPid->fMax_Out;
//	
//    return _tPid->fCtrl_Out;
//}

/**
 * @brief PID�������㺯��
 * @param _tPid-PID�ṹ��ָ��
 * @param _fCurrValue-��ǰֵ
 * @param _fExpValue-����ֵ
 * @retval int ����PID���������ֵ
*/
int PID_Add_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue)
{
    int Delt_Out; /* PID�������ֵ */
	float Pre_I;

    /* �趨����ֵ�͵�ǰֵ */
    _tPid->fExp_Value  = _fExpValue;
    _tPid->fCurr_Value = _fCurrValue;
    
	/* ��Ź�ȥ�������ֵ */
    _tPid->fError[2] = _tPid->fError[1];
    _tPid->fError[1] = _tPid->fError[0];
    _tPid->fError[0] = _fExpValue - _fCurrValue;
	
	/* ��¼��ȥ��Iout���ֵ�����ڵ�ͨ�˲� */
	Pre_I = _tPid->fIout;

    /* ��������ʽPID */

    /* �Ա���������ϴ����Ĳ�ֵ��Ϊ����������������� */
    _tPid->fPout = _tPid->fKp * (_tPid->fError[0] - _tPid->fError[1]);

    /* �Ա��������Ϊ������������ */
    _tPid->fIout = _tPid->fKi * _tPid->fError[0];
	/* �Ի�������õ�ͨ�˲� */
	_tPid->fIout = _tPid->alpha * _tPid->fIout + (1 - _tPid->alpha) * Pre_I;

	//�޷�
	if(_tPid->fIout > _tPid->fMax_Iout)
		_tPid->fIout = _tPid->fMax_Iout;

    /* ����΢��������� */
    _tPid->fDbuf[2] = _tPid->fDbuf[1];
    _tPid->fDbuf[1] = _tPid->fDbuf[0];

    /* �Ա���������ϴ����Ĳ�ֵ��ȥ�ϴ���������ϴ����Ĳ�ֵ��Ϊ΢��������������� */
    _tPid->fDbuf[0] = (_tPid->fError[0] - 2.0f * _tPid->fError[1] + _tPid->fError[2]);
    _tPid->fDout = _tPid->fKd * _tPid->fDbuf[0];
    
    /* ������� */
    Delt_Out = (int)(_tPid->fPout + _tPid->fIout + _tPid->fDout);    /* ����������� */
	
	//�޷�
	if(Delt_Out > _tPid->fMax_Out)
		Delt_Out = _tPid->fMax_Out;
	
    _tPid->fCtrl_Out = _tPid->fPre_Out + Delt_Out;                 /* ��ǰֵ�ۼ� */
    _tPid->fPre_Out = _tPid->fCtrl_Out;                            /* ��¼��ֵ��Ϊ�´�PID��׼�� */


    return (int)_tPid->fCtrl_Out;
}

/**
 * @brief PIDλ�ü��㺯��
 * @param _tPid-PID�ṹ��ָ��
 * @param _fCurrValue-��ǰֵ
 * @param _fExpValue-����ֵ
 * @retval int ����PID���������ֵ
*/
int PID_Location_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue)
{
	
	
	/* �趨����ֵ�͵�ǰֵ */
    _tPid->fExp_Value  = _fExpValue;
    _tPid->fCurr_Value = _fCurrValue;
	
	/* ��Ź�ȥ�������ֵ */
    _tPid->fError[2] = _tPid->fError[1];
    _tPid->fError[1] = _tPid->fError[0];
    _tPid->fError[0] = _fExpValue - _fCurrValue;
	
	/* ����λ��ʽPID */
	
	/* ����P���ֵ */
	_tPid->fPout = _tPid->fKp * _tPid->fError[0];
	
	/* ����D���ֵ */
	_tPid->fDout = _tPid->fKd * (_tPid->fError[0] - _tPid->fError[1]);
	
	/* ����I���ֵ */
	_tPid->fIout += _tPid->fKi * _tPid->fError[0];
	
	//�����޷�
	if(_tPid->fIout > _tPid->fMax_Iout)
		_tPid->fIout = _tPid->fMax_Iout;
	
	//�������
	_tPid->fCtrl_Out = _tPid->fPout + _tPid->fIout + _tPid->fDout;
	
//	_tPid->fCtrl_Out = _tPid->fCtrl_Out*0.8 + _tPid->fPre_Out*0.2;
	
	//����޷�
	if(_tPid->fCtrl_Out > _tPid->fMax_Out)
		_tPid->fCtrl_Out = _tPid->fMax_Out;
	
//	_tPid->fPre_Out = _tPid->fCtrl_Out;
	
	return (int)_tPid->fCtrl_Out;
	
}

/**
 * @brief PID��ʷ����������պ���
 * @param _tPid-PID�ṹ��ָ��
 * @retval Null
*/
void PID_Clear(tagPID_T *_tPid)
{
	/* ��ǰ������� */
    _tPid->fError[0] = _tPid->fError[1] = _tPid->fError[2] = 0.0f;

    /* ΢�������� */
    _tPid->fDbuf[0] = _tPid->fDbuf[1] = _tPid->fDbuf[2] = 0.0f;

    /* ������� */
    _tPid->fCtrl_Out = _tPid->fPout = _tPid->fIout = _tPid->fDout = 0.0f;

    /* Ŀ��ֵ�͵�ǰֵ���� */
    _tPid->fCurr_Value = _tPid->fExp_Value = 0.0f;
}

