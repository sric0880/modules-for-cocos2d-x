//
//  CustomHelper.h
//
//  Created by qiong on 14-3-22.
//
//

#ifndef CustomHelper_h
#define CustomHelper_h
#include <cocostudio/CCSGUIReader.h>
#include "CustomInput.h"
#include "CustomInputReader.h"
#include "CustomLabel.h"
#include "CustomLabelReader.h"
#include "CustomButton.h"
#include "CustomButtonReader.h"
using namespace cocostudio;

inline void registerCustomWidget()
{
    GUIReader* guiReader = GUIReader::getInstance();
    guiReader->registerTypeAndCallBack("CustomInput",
                                       &CustomInput::createInstance,
                                       CustomInputReader::getInstance(),
                                       parseselector(CustomInputReader::setProperties));
    guiReader->registerTypeAndCallBack("CustomLabel",
                                       &CustomLabel::createInstance,
                                       CustomLabelReader::getInstance(),
                                       parseselector(CustomLabelReader::setProperties));
    guiReader->registerTypeAndCallBack("CustomButton",
                                       &CustomButton::createInstance,
                                       CustomButtonReader::getInstance(),
                                       parseselector(CustomButtonReader::setProperties));
}

#endif
