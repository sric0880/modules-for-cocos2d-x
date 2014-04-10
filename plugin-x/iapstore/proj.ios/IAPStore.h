//
//  PluginIAPStore.h
//  PluginIAPStore
//
//  Created by qiong on 14-4-10.
//  Copyright (c) 2014年 qiong. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InterfaceIAP.h"

typedef enum{
    IAPRequestStateLoading = 1,
    IAPRequestStateFail,
    IAPRequestStateSuccess
} IAPRequestState;

@interface IAPStore : NSObject <InterfaceIAP>
{
}

@property(assign,nonatomic)IAPRequestState iapRequestState;
@property(nonatomic,retain)NSArray *products;
@property(nonatomic,retain)NSArray *productIdentfiers;

/*
 Interface from InterfaceIAP
 */
- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo;
- (void) payForProduct: (NSMutableDictionary*) profuctInfo;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

/*
 Custom interface
 */
+(bool)canMakePay; //是否禁用商城

@end
