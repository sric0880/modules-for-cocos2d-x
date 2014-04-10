//
//  PluginIAPStore.m
//  PluginIAPStore
//
//  Created by qiong on 14-4-10.
//  Copyright (c) 2014年 qiong. All rights reserved.
//

#import "IAPStore.h"
#import <StoreKit/StoreKit.h>
#import "IAPWrapper.h"

@interface IAPStore() <SKProductsRequestDelegate,SKPaymentTransactionObserver>
{
    SKProductsRequest *productsRequest;
    int buyindex;
}
@end

@implementation IAPStore

@synthesize iapRequestState;
@synthesize products;
@synthesize productIdentfiers;

- (void)dealloc
{
    [products release];
    [productIdentfiers release];
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

- (id)init
{
    self = [super init];
    if (self) {
        buyindex = -1;
        self.iapRequestState = IAPRequestStateFail;
    }
    return self;
}

+(bool)canMakePay
{
    return [SKPaymentQueue canMakePayments];
}

#pragma mark InterfaceIAP impl
- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo
{
    NSString* idstr = [cpInfo objectForKey:@"ProductIdentifiers"];
    self.productIdentfiers = [idstr componentsSeparatedByString:@","];
    for(NSString *identifier in self.productIdentfiers) {
        NSLog(@"identifier: %@", identifier);
    }
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [self requestProductList];
}
- (void) payForProduct: (NSMutableDictionary*) profuctInfo
{
    //TODO: 判断是否有网
//    [self payResult:kPayFail withMsg:@"NetWork_Unreachable"];
    buyindex = [[profuctInfo objectForKey:@"productId"] intValue];
    if (self.iapRequestState == IAPRequestStateSuccess) {
        [self dealBuy];
    }else if (self.iapRequestState == IAPRequestStateLoading) {
        NSLog(@"正在请求商品列表");
    }else{
        NSLog(@"重新请求商品列表");
        [self requestProductList];
    }
}
- (void) setDebugMode: (BOOL) debug
{
    
}
- (NSString*) getSDKVersion
{
    return @"1.0.0";
}
- (NSString*) getPluginVersion
{
    return @"1.0.0";
}

-(void)payResult:(PayResult)ret withMsg:(NSString *) msg
{
    [IAPWrapper onPayResult:self withRet:ret withMsg:msg];
}

-(void)requestProductList{
    self.iapRequestState = IAPRequestStateLoading;
    NSSet *productIdentifiers = [NSSet setWithArray:self.productIdentfiers ];
    productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
}

-(void)dealBuy{
    SKProduct *product = nil;
    for (SKProduct * p in self.products) {
        if ([p.productIdentifier isEqualToString:[self.productIdentfiers objectAtIndex:buyindex]]) {
            product = p;
            break;
        }
    }
    if (product!=nil) {
        SKPayment *payment = [SKPayment paymentWithProduct:product];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }
}

- (void)finishTransaction:(SKPaymentTransaction *)transaction
{
    // remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    [self payResult:kPaySuccess withMsg:@"AppStore_OK"];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        // error!
        [self payResult:kPayFail withMsg:@"AppStore_Error"];
        NSLog(@"%@/n%@",[transaction.error localizedDescription],[transaction.error localizedFailureReason]);
    }
    else
    {
        // this is fine, the user just cancelled, so don’t notify
        [self payResult:kPayCancel withMsg:@"AppStore_Cancelled"];
    }
}

#pragma mark SKPaymentTransactionObserver impl
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0)
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
            case SKPaymentTransactionStateRestored:
                [self finishTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            default:
                break;
        }
    }
}

#pragma mark SKProductsRequestDelegate impl
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response __OSX_AVAILABLE_STARTING(__MAC_NA,__IPHONE_3_0)
{
    self.products = response.products;
    NSLog(@"invalid product identifier: %@",response.invalidProductIdentifiers);
    
    if(self.products.count==0){
        self.iapRequestState = IAPRequestStateFail;
        if(buyindex!=-1){
            [self payResult:kPayFail withMsg:@"Products_List_Error"];
        }
    }else{
        self.iapRequestState = IAPRequestStateSuccess;
        if(buyindex!=-1){
            [self dealBuy];
        }
    }
}

@end
