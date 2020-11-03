#pragma once

#include "CoreMinimal.h"
#include "VersionInfo.h"
#include "Runtime/Launch/Resources/Version.h"

namespace WeTestU3DAutomation
{
	namespace Cmd
	{
		static const ::int32 EXIT = 0;//�˳���Ϸ
									////////////////////////
		static const ::int32 GET_VERSION = 100;//��ȡ�汾��
		static const ::int32 FIND_ELEMENTS = 101;//���ҽڵ�
		static const ::int32 FIND_ELEMENT_PATH = 102;//ģ������
		static const ::int32 GET_ELEMENTS_BOUND = 103;//��ȡ�ڵ��λ����Ϣ
		static const ::int32 GET_ELEMENT_WORLD_BOUND = 104;//��ȡ�ڵ����������
		static const ::int32 GET_UI_INTERACT_STATUS = 105;//��ȡ��Ϸ�Ŀɵ����Ϣ������scene���ɵ���ڵ㣬��λ����Ϣ
		static const ::int32 GET_CURRENT_SCENE = 106;//��ȡUnity��Scene����
		static const ::int32 GET_ELEMENT_TEXT = 107;//��ȡ�ڵ����������
		static const ::int32 GET_ELEMENT_IMAGE = 108;//��ȡ�ڵ��ͼƬ����
		static const ::int32 GET_REGISTERED_HANDLERS = 109;//��ȡע��ĺ���������
		static const ::int32 CALL_REGISTER_HANDLER = 110;//����ע��ĺ���
		static const ::int32 SET_INPUT_TEXT = 111;//input�ؼ�����������Ϣ
		static const ::int32 GET_OBJECT_FIELD = 112;//�����ȡ��������ֵ
		static const ::int32 FIND_ELEMENTS_COMPONENT = 113;//��ȡ���а����������gameobject
		static const ::int32 SET_CAMERA_NAME = 114;//������Ⱦ����ѵ�Camera
		static const ::int32 GET_COMPONENT_METHODS = 115;//�����ȡ����ϵķ���
		static const ::int32 CALL_COMPONENT_MOTHOD = 116;//ͨ�������������ĺ���
		static const ::int32 LOAD_TEST_LIB = 117;//����test��صĿ�

		static const ::int32 PRC_SET_METHOD = 118;//ע��python�˵ķ���
		static const ::int32 RPC_METHOD = 119;//��Ϸ�ڵĽӿڿɵ��ã�python�˵ķ���

		///////////////////////////////////////////////
		static const ::int32 HANDLE_TOUCH_EVENTS = 200;//����down;move;up

		//////////////////////////////////////////////
		static const ::int32 DUMP_TREE = 300;//��ȡ�ڵ���xml
		static const ::int32 FIND_ELEMENT_BY_POS = 301;//����λ����Ϣ��ȡ�ڵ�����

		//////////////////////////////////////////////
		static const ::int32 GET_FPS = 400;//��ȡFPS
		static const ::int32 GET_TRAFFIC_DATA = 401;//��ȡ����

		//////////////////////////////////////////////
		static const ::int32 ENTER_RECORD = 500;//��ʼ¼��
		static const ::int32 LEAVE_RECORD = 501;//����¼��
		static const ::int32 TOUCH_NOTIFY = 502;//���ص���Ľڵ�
	};

	enum class ResponseStatus :int32 {
		SUCCESS = 0,
		NO_SUCH_CMD = 1,//���͵������ʶ��
		UNPACK_ERROR = 2,//������body���ݣ�json�����л�ʧ��
		UN_KNOW_ERROR = 3,//һ������ִ������Ĺ������׳��쳣��δԤ�ϵ�����
		GAMEOBJ_NOT_EXIST = 4,//gameobject ������
		COMPONENT_NOT_EXIST = 5,//component ������
		NO_SUCH_HANDLER = 6,//û�и��Զ����handler

		REFLECTION_ERROR = 7,//�����ȡĳ������ʱ����
		NO_SUCH_RESOURCE = 8,//û���������Դ����
	};

	enum class ResponseDataType :uint8 
	{
		STRING=0,
		OBJECT=1
	};

	struct FCommand {
		int32 cmd;
		ResponseStatus status;
		FString ResponseJson;
		ResponseDataType ReponseJsonType;


		FCommand(){
			cmd = 0;
			status = ResponseStatus::SUCCESS;
			ReponseJsonType = ResponseDataType::STRING;
		};

		FString ToJson();
	};

	struct FVersionData
	{
		FString sdkVersion;//SDK�汾����1.0.1,���汾.�Ӱ汾.�����汾
		FString engine;
		FString engineVersion;
		FString sdkUIType;//UGUI,NGUI

		FVersionData()
		{
			sdkVersion = SDK_VERSION;
			engine = ENGINE;
			sdkUIType = SDK_UI;
			engineVersion = FString::Printf(TEXT("%d.%d.%d"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION);
		}

		FString ToJson();
	};

	struct FDumpTree
	{
		FString scene;
		FString xml;

		FString ToJson();
	};

	struct FElementInfo
	{
		int64 instance;
		FString name;

		FString ToJson();
	};

	struct FBoundInfo
	{
		int64 instance;
		bool visible;
		bool existed;
		float width;
		float height;
		float x;
		float y;
		FString path;

		FBoundInfo():existed(true), visible(true),path(""),width(0.0f),height(0.0f),x(0.0f),y(0.0f)
		{
		};

		FString ToJson();
	};
}