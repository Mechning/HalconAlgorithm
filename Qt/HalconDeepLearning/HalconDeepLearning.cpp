#include "HalconDeepLearning.h"
#include "HalconCpp.h"

using namespace HalconCpp;
extern void check_dl_devices(HTuple *hv_PossibleRuntimes);
extern void create_dl_preprocess_param_from_model (HTuple hv_DLModelHandle, HTuple hv_NormalizationType, 
    HTuple hv_DomainHandling, HTuple hv_SetBackgroundID, HTuple hv_ClassIDsBackground, 
    HTuple hv_GenParam, HTuple *hv_DLPreprocessParam);

QImage HalconDeepLearning::hImage2Pixmap(QSharedPointer<HalconCpp::HImage> image)
{
	if (!image || !image->IsInitialized()) {//δ��ʼ������ͼ���ʼ���򷵻ؿ�
		return {};
	}
	auto channelCount{ image->CountChannels() };
	if (channelCount==1) {//��ͨ���Ҷ�ͼ��
		Hlong width, height;//ͼ����
		HString hType;
		auto imgPtr{static_cast<uchar*>(image->GetImagePointer1(&hType,&width,&height)) };
		//����QImage
		return QImage( imgPtr,width,height,QImage::Format_Indexed8 );
	}
}


HalconDeepLearning::HalconDeepLearning(QObject *parent /*= Q_NULLPTR*/)
	:hv_DLModelHandle_(new HTuple),\
	hv_DLPreprocessParam_(new HTuple),\
	hv_classNames_(new HTuple),\
	batchSize_(1),\
	threadCount_(4)
{
}

HalconDeepLearning::~HalconDeepLearning()
{
	delete hv_DLModelHandle_;
	delete hv_DLPreprocessParam_;
	delete hv_classNames_;
}

void HalconDeepLearning::setModelPath(const QString&modelPath)
{
	modelPath_ = modelPath;
}

QString HalconDeepLearning::getModelPath()
{
	return modelPath_;
}

void HalconDeepLearning::setBatchSize(int size)
{
	batchSize_ = size;
}

int HalconDeepLearning::getBatchSize(int size)
{
	return batchSize_;
}

void HalconDeepLearning::setThreadCount(int count)
{
	threadCount_ = count;
}

int HalconDeepLearning::getThreadCount()
{
	return threadCount_;
}

void HalconDeepLearning::initDlModel(QString modelPath,int batchSize,int threadCount)
{
	if (!modelPath.isEmpty()) {
		setModelPath(modelPath);
	}
	if (batchSize) {
		setBatchSize(batchSize);
	}
	if (threadCount) {
		setThreadCount(threadCount);
	}

	HTuple  hv_modelFile{ &(modelPath_.toStdString()[0]) };//ģ��·��
	//����ѵ���õ�����Model
	ReadDlModel(hv_modelFile, hv_DLModelHandle_);
	//���ģ������
	GetDlModelParam(*hv_DLModelHandle_, "class_names", hv_classNames_);
	//�������õ�Ӳ��(cpu/gpu)
	HTuple hv_PossibleRuntimes;
	check_dl_devices(&hv_PossibleRuntimes);
	//��gpu������Ϊgpuѵ��
	HTuple hv_Runtime;//����ʱʹ��gpu/cpu
	if (0 != (hv_PossibleRuntimes.TupleRegexpTest("gpu")))
	{
		hv_Runtime = "gpu";
	}
	else if (0 != (hv_PossibleRuntimes.TupleRegexpTest("cpu")))
	{
		hv_Runtime = "cpu";
		//�趨cpu�߳���
		SetSystem("thread_num", threadCount_);//ʹ��4���߳�
	}
	else
	{
		throw HException("No supported device found to continue this example.");
	}
	SetDlModelParam(*hv_DLModelHandle_, "runtime", hv_Runtime);
	//ÿ4����Ƭһ�����
	SetDlModelParam(*hv_DLModelHandle_, "batch_size", batchSize_);
	//����Ԥ�������Ϣ�ֵ�
	create_dl_preprocess_param_from_model(*hv_DLModelHandle_, "none", "full_domain",
		HTuple(), HTuple(), HTuple(), hv_DLPreprocessParam_);
}
