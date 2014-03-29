#pragma once

#include "CSWidget.h"
#include "CustomLabel.h"

using namespace cocos2d;

namespace CocoStudio{
	namespace EngineAdapter{

		class CSCustomLabel : public CSWidget
		{
		private:
			CustomLabel* GetInnerObject();
			std::string textId;
			std::string ttfConfig;
			std::string bmfConfig;
		public:
			CSCustomLabel();
			~CSCustomLabel();

			virtual void InitPropertyEx()override;
			virtual void AfterAdded()override;

			void CS_SetTextId(const char* textid);
			const char* CS_GetTextId();
			void CS_SetTTFConfigId(const char* id);
			const char* CS_GetTTFConfigId();
			void CS_SetBMFConfigId(const char* id);
			const char* CS_GetBMFConfigId();

			void CloneWidgetCustomProperty(CSWidget* cWidget) override;
		};

	}
}
