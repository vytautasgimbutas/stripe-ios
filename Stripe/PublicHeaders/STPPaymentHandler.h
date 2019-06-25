//
//  STPPaymentHandler.h
//  StripeiOS
//
//  Created by Cameron Sabol on 5/10/19.
//  Copyright © 2019 Stripe, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class STPAPIClient;
@class STPPaymentIntent;
@class STPPaymentIntentParams;
@class STPThreeDSCustomizationSettings;
@protocol STPAuthenticationContext;

/**
 `STPPaymentHandlerActionStatus` represents the possible outcomes of requesting an action by `STPPaymentHandler`. An action could be confirming and/or handling the next action for a PaymentIntent.
 */
typedef NS_ENUM(NSInteger, STPPaymentHandlerActionStatus) {
    /**
     The action succeeded.
     */
    STPPaymentHandlerActionStatusSucceeded,

    /**
     The action was cancelled by the cardholder/user.
     */
    STPPaymentHandlerActionStatusCanceled,

    /**
     The action failed. See the error code for more details.
     */
    STPPaymentHandlerActionStatusFailed,
};

/**
 The error domain for errors in `STPPaymentHandler`.
 */
FOUNDATION_EXPORT NSString * const STPPaymentHandlerErrorDomain;

/**
 Error codes generated by `STPPaymentHandler`
 */
typedef NS_ENUM(NSInteger, STPPaymentHandlerErrorCode) {
    /**
     Indicates that the action requires an authentication method not recognized or supported by the SDK.
     */
    STPPaymentHandlerUnsupportedAuthenticationErrorCode,

    /**
     The PaymentIntent could not be confirmed because it is missing an associated payment method.
     */
    STPPaymentHandlerRequiresPaymentMethodErrorCode,

    /**
     The PaymentIntent status cannot be resolved by `STPPaymentHandler`.
     */
    STPPaymentHandlerPaymentIntentStatusErrorCode,

    /**
     The action timed out.
     */
    STPPaymentHandlerTimedOutErrorCode,

    /**
     There was an error in the Stripe3DS2 SDK.
     */
    STPPaymentHandlerStripe3DS2ErrorCode,

    /**
     The transaction did not authenticate (e.g. user entered the wrong code).
     */
    STPPaymentHandlerNotAuthenticatedErrorCode,

    /**
     `STPPaymentHandler` does not support concurrent actions.
     */
    STPPaymentHandlerNoConcurrentActionsErrorCode,

    /**
     Payment requires an `STPAuthenticationContext`.
     */
    STPPaymentHandlerRequiresAuthenticationContextErrorCode,
};


/**
 Completion block typedef for use in `STPPaymentHandler` methods.
 */
typedef void (^STPPaymentHandlerActionCompletionBlock)(STPPaymentHandlerActionStatus, STPPaymentIntent * _Nullable, NSError * _Nullable);

/**
 `STPPaymentHandler` is a utility class that can confirm PaymentIntents and handle
 any additional required actions for 3DS(2) authentication. It can present authentication UI on top of your app or redirect users out of your app (to e.g. their banking app).
 */
NS_EXTENSION_UNAVAILABLE("STPPaymentHandler is not available in extensions")
@interface STPPaymentHandler : NSObject

/**
 The globally shared instance of `STPPaymentHandler`.
 */
+ (instancetype)sharedHandler;

/**
 By default `sharedHandler` initializes with [STPAPIClient sharedClient].
 */
@property (nonatomic) STPAPIClient *apiClient;

/**
 Customizable settings to use when performing 3DS2 authentication.
 
 Note: Configure this before calling any methods.
 Defaults to `[STPThreeDSCustomizationSettings defaultSettings]`.
 */
@property (nonatomic) STPThreeDSCustomizationSettings *threeDSCustomizationSettings;

/**
 Confirms the PaymentIntent with the provided parameters and handles any `nextAction` required
 to authenticate the PaymentIntent.
 */
- (void)confirmPayment:(STPPaymentIntentParams *)paymentParams
withAuthenticationContext:(nullable id<STPAuthenticationContext>)authenticationContext
            completion:(STPPaymentHandlerActionCompletionBlock)completion;

/**
 Handles any `nextAction` required to authenticate the PaymentIntent.
 */
- (void)handleNextActionForPayment:(STPPaymentIntent *)paymentIntent
         withAuthenticationContext:(id<STPAuthenticationContext>)authenticationContext
                        completion:(STPPaymentHandlerActionCompletionBlock)completion;

@end

NS_ASSUME_NONNULL_END
