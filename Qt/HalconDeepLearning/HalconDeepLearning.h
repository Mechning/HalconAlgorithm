#pragma once
#include<QObject>
#include<QSharedPointer>
#include<QImage>
#include "halcondeeplearning_global.h"

namespace HalconCpp { 
	class HImage;
	class HTuple;
}

class HALCONDEEPLEARNING_EXPORT HalconDeepLearning:public QObject
{
	Q_OBJECT

public:
	static QImage hImage2Pixmap(QSharedPointer<HalconCpp::HImage> image);//ת��HImage��QPixmap

	HalconDeepLearning(QObject *parent = Q_NULLPTR);
	virtual ~HalconDeepLearning();
	virtual void initDlModel(QString modelPath="",int batchSize=0,int threadCount=0);//��ʼ��ģ��
	void setModelPath(const QString&modelPath);//����ģ��·��
	QString getModelPath();//���ģ��·��
	void setBatchSize(int size);//������������
	int getBatchSize(int size);//�����������
	void setThreadCount(int count);//ʹ��cpuʱʹ�õ��߳���,Ĭ��ʹ��gpu,û����ʹ��cpu
	int getThreadCount();//���cpuʹ�õ��߳���

protected:

	HalconCpp::HTuple *hv_DLModelHandle_;//ģ�;��
	HalconCpp::HTuple *hv_DLPreprocessParam_;//Ԥ������Ϣ
	HalconCpp::HTuple * hv_classNames_;//ģ������
	QString modelPath_;//ģ�͵�·��
	int batchSize_;//��������
	int threadCount_;//cpuʹ���߳���
};
