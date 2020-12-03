// Dynatrace.h
// Version: 8.205.5.1011
//
// These materials contain confidential information and
// trade secrets of Dynatrace Corporation. You shall
// maintain the materials as confidential and shall not
// disclose its contents to any third party except as may
// be required by law or regulation. Use, disclosure,
// or reproduction is prohibited without the prior express
// written permission of Dynatrace LLC.
//
// All Dynatrace products listed within the materials are
// trademarks of Dynatrace Corporation. All other company
// or product names are trademarks of their respective owners.
//
// Copyright 2011-2016 Dynatrace LLC

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <UIKit/UIKit.h>

/*!
 @file Dynatrace.h
 @abstract This is the file developers include in their projects to use the Dynatrace OneAgent.
 @discussion This file provides the declarations of the Dynatrace and DTXAction classes and should
 be included in any iOS source file that uses the Dynatrace OneAgent.
 */

#ifndef DYNAFRAMEWORK
#define DYNAFRAMEWORK

/*!
 @typedef DTX_DataCollectionLevel
 @abstract This enum represents the different privacy levels that the user can select.
 @discussion The selected data collection level has no influence on crash reporting, because crash reporting is configured by a different setting.
 @constant DTX_DataCollectionOff The agent does not capture data.
 @constant DTX_DataCollectionPerformance The agent only captures anonymous performance data.
 @constant DTX_DataCollectionUserBehavior The agent captures performance and user data.
 */
typedef NS_ENUM(int, DTX_DataCollectionLevel) {
    DTX_DataCollectionOff,
    DTX_DataCollectionPerformance,
    DTX_DataCollectionUserBehavior
};

/*!
 @typedef DTX_StatusCode
 @abstract Usage of status codes is discouraged, it will be removed in a future release.
 @constant DTX_CaptureOff OneAgent is not enabled or can't capture data.
 @constant DTX_CaptureOn OneAgent is enabled.
 @constant DTX_CrashReportingUnavailable PL Crash Reporter framework is unavailable to OneAgent.
 @constant DTX_CrashReportingAvailable PL Crash Reporter framework is available to OneAgent.
 @constant DTX_Error_NotInitialized OneAgent is not initialized.
 @constant DTX_Error_InvalidRange Parameter value specified is outside of permitted range.
 @constant DTX_Error_InternalError An internal error occurred.
 @constant DTX_Error_ActionNotFound A corresponding enterAction event was not found for the current leaveAction.
 @constant DTX_Error_InvalidParameter A null or 0 length serverURL, applicationName, actionName, or eventName are used.
 @constant DTX_Error_ActionEnded The action has already been ended via the leaveAction method.
 @constant DTX_ReportErrorOff Returned if the DT server has turned error reporting off.
 @constant DTX_TruncatedEventName Returned if the actionName or eventName exceeds maximum length.
 @constant DTX_CrashReportInvalid Crash Report was invalid.
 @constant DTX_TruncatedUserId Returned if the userid exceeds maximum length.
 */
typedef NS_ENUM(int, DTX_StatusCode) {
    DTX_CaptureOff = 1,
    DTX_CaptureOn = 2,
    DTX_CrashReportingUnavailable = 4,
    DTX_CrashReportingAvailable = 5,
    DTX_Error_NotInitialized = -1,
    DTX_Error_InvalidRange = -2,
    DTX_Error_InternalError = -3,
    DTX_Error_ActionNotFound = -4,
    DTX_Error_InvalidParameter = -5,
    DTX_Error_ActionEnded = -6,
    DTX_ReportErrorOff = -8,
    DTX_TruncatedEventName = -9,
    DTX_CrashReportInvalid = -10,
    DTX_TruncatedUserId = -11,
} /*__attribute__((deprecated))*/;

#if __has_feature(nullability)
#define NULLABLE nullable
#define NONNULL nonnull
#else
#define NULLABLE
#define NONNULL
#endif

/*!
 @category UIControl (DynatraceCustomization)
 @abstract Category for changing the instrumentation behaviour of UIControls.
 */
@interface UIControl (DynatraceCustomization)

/*!
 @brief Sets a custom name for the control.
 
 The custom name is used instead of the captured control title. This can be used to customise control names or hide them for privacy reasons.
 Providing an empty String will use the control type as name (e.g. "Button"). Providing nil will reset to default behaviour.
 @param name The custom name that will be used instead of the captured control title.
 */
- (void)dtxCustomControlName:(NULLABLE NSString *)name;

@end

/*!
@category UITableViewCell (DynatraceCustomization)
@abstract Category for changing the instrumentation behaviour of UITableViewCells.
*/
@interface UITableViewCell (DynatraceCustomization)

/*!
 @brief Sets a custom name for the table cell.
 
 The custom name is used instead of the captured cell title. This can be used to customise cell names or hide them for privacy reasons.
 Providing an empty String will use the cell type as name (e.g. "UITableCell"). Providing nil will reset to default behaviour.
 @param name The custom name that will be used instead of the captured cell title.
 */
- (void)dtxCustomCellName:(NULLABLE NSString *)name;

@end

/*!
@category UICollectionViewCell (DynatraceCustomization)
@abstract Category for changing the instrumentation behaviour of UICollectionViewCells.
*/
@interface UICollectionViewCell (DynatraceCustomization)

/*!
 @brief Sets a custom name for the table cell.
 
 The custom name is used instead of the captured cell title. This can be used to customise cell names or hide them for privacy reasons.
 Providing an empty String will use the cell type as name (e.g. "CollectionViewCell"). Providing nil will reset to default behaviour.
 @param name The custom name that will be used instead of the captured cell title.
 */
- (void)dtxCustomCellName:(NULLABLE NSString *)name;

@end

/*!
 @class DTXAction
 @abstract This class encapsulates a timed mobile action.
 @discussion It creates or extends a mobile action PurePath in Dynatrace. It provides methods to report values,
 events, and errors against actions.
 */
@interface DTXAction : NSObject

/*!
 @brief Starts a top level action.

 The top level action results in a new mobile action PurePath in Dynatrace. An action allows you
 to time an interval in your code. Call this method at the point you want to start timing.
 Call the leaveAction instance method on the returned object at the point you want to stop timing.

 @param actionName Name of action

 @return Returns the new action or nil if an error occurs.
 */
+ (NULLABLE DTXAction*)enterActionWithName:(NONNULL NSString*)actionName;

/*!
 @brief Starts a child action.

 The action adds a node to an existing mobile action PurePath in Dynatrace. An action allows you
 to time an interval in your code. Call this method at the point you want to
 start timing. Call the leaveAction instance method on the returned object at the point you want
 to stop timing.

 @param actionName Name of action

 @param parentAction The parent action for this action.

 @return Returns the new action or nil if an error occurs.
 */
+ (NULLABLE DTXAction*)enterActionWithName:(NONNULL NSString*)actionName parentAction:(NULLABLE DTXAction*)parentAction;

/*!
 @brief Ends an action and computes its interval.

 All reported events, values, and tagged web requests between start and end of an action are
 nested in the mobile action PurePath. If this action has any child actions, they are ended
 first. Call this method at the end of the code that you wish to time. The number of milliseconds
 since the action began is stored as the interval.

 In non-ARC code the DTXAction must be released after calling this method.

 @return Returns a DTX_StatusCode indicating success (DTX_CaptureOn) or failure
 */
- (DTX_StatusCode)leaveAction;

/*!
 @brief Sends an event to Dynatrace.

 The event becomes a node of the mobile action PurePath.

 @param eventName Name of event

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportEventWithName:(NONNULL NSString*)eventName;

/*!
 @brief Sends a key/value pair to Dynatrace.

 The value becomes a node of the mobile action PurePath. The value can be processed by a measure and
 thus be charted.

 @param valueName Name of value

 @param intValue An integer value associated with the value name

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportValueWithName:(NONNULL NSString*)valueName intValue:(int64_t)intValue;

/*!
 @brief Sends a key/value pair to Dynatrace.

 The value becomes a node of the mobile action PurePath. The value can be processed by a measure and
 thus be charted.

 @param valueName Name of value

 @param doubleValue A double value associated with the value name

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportValueWithName:(NONNULL NSString*)valueName doubleValue:(double)doubleValue;

/*!
 @brief Sends a key/value pair to Dynatrace.

 The value becomes a node of the mobile action PurePath.

 @param valueName Name of value

 @param stringValue A string value associated with the value name

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportValueWithName:(NONNULL NSString*)valueName stringValue:(NONNULL NSString*)stringValue;

/*!
 @brief Sends an error value to Dynatrace.

 The error becomes a node of the mobile action PurePath.

 @param errorName Name of error

 @param errorValue An integer value associated with the error

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportErrorWithName:(NONNULL NSString*)errorName errorValue:(int)errorValue;

/*!
 @brief Sends an exception to Dynatrace.

 The exception becomes a node of the mobile action PurePath. This can be used to report exceptions
 that are caught and handled.

 @param errorName Name of error

 @param exception An exception object that has been caught. The description string of this
 object is sent to the server along with the call stack if one is present.

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportErrorWithName:(NONNULL NSString*)errorName exception:(NONNULL NSException*)exception;

/*!
 @brief Sends an error object to Dynatrace.

 The error becomes a node of the mobile action PurePath.

 @param errorName Name of error

 @param error An error object that has been caught. The error information for this
 object is sent to the server.

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)reportErrorWithName:(NONNULL NSString*)errorName error:(NONNULL NSError*)error;

/*!
 @brief Sends an error value to Dynatrace.

 Because this is a class method, the error is not associated with an action. It creates
 its own mobile-only PurePath.

 @param errorName Name of error

 @param errorValue An integer value associated with the error

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)reportErrorWithName:(NONNULL NSString*)errorName errorValue:(int)errorValue;

/*!
 @brief Sends an exception to Dynatrace.

 Because this is a class method, the exception is not associated with an action. It creates
 its own mobile-only PurePath.

 @param errorName Name of error

 @param exception An exception object that has been caught. The description string of this
 object is sent to the server along with the call stack if one is present.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)reportErrorWithName:(NONNULL NSString*)errorName exception:(NONNULL NSException*)exception;

/*!
 @brief Sends an error object to Dynatrace.

 Because this is a class method, the error is not associated with an action. It creates
 its own mobile-only PurePath.

 @param errorName Name of error

 @param error An error object that has been caught. The error information for this
 object is sent to the server.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)reportErrorWithName:(NONNULL NSString*)errorName error:(NONNULL NSError*)error;

@property (readonly) int tagId;

@end

/*************************************************************************************************/

/*!
 @brief Provides the ability to manually tag and time web requests.

 The DTXWebRequestTiming class provides the ability to manually time web requests.
 */
@interface DTXWebRequestTiming : NSObject

/*!
 @brief Creates a DTXWebRequestTiming object.

 This method creates an instance of the DTXWebRequestTiming object.

 @param requestTagString the value of the HTTP header that you must add to your web request. this
 is obtained from the Dynatrace getRequestTagValueForURL method.

 @param requestUrl the value of the URL for the web request

 @return Returns the DTXWebRequestTiming object.
 */
+ (NULLABLE DTXWebRequestTiming*)getDTXWebRequestTiming:(NONNULL NSString*)requestTagString requestUrl:(NULLABLE NSURL*)requestUrl;

/*!
 @brief Manually start timing a web request.

 The Dynatrace OneAgent automatically times web requests made using NSURLRequest, NSURLConnection,
 NSURLProtocol and NSString. If you use an alternate technology to make
 web requests and want to time them, use the getRequestTagHeader method, adding that information to
 your request, and then this method to start the timing.

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)startWebRequestTiming;

/*!
 @brief Manually finish timing a web request.

 @param statusCode the response status code for a successful web request or the error code or error description
 for a failed web request

 The Dynatrace OneAgent automatically times web requests made using NSURLRequest, NSURLConnection,
 NSURLProtocol and NSString. If you use an alternate technology to make
 web requests and want to time them, use the getRequestTagHeader method, adding that information to
 your request, and then this method to stop the timing and send the information to the mobile action PurePath.

 @return Returns a DTX_StatusCode
 */
- (DTX_StatusCode)stopWebRequestTiming:(NULLABLE NSString*)statusCode;

@end

/*************************************************************************************************/


/*!
@brief DTXUserPrivacyOptions protocol should be implemented by objects passed to applyUserPrivacyOptions:completion: method.
@property crashReportingOptedIn current privacy setting for crash reporting
@property dataCollectionLevel the current data collection level.
*/

@protocol DTXUserPrivacyOptions <NSObject>
@required
@property (nonatomic) BOOL crashReportingOptedIn;
@property (nonatomic) BOOL crashReplayOptedIn;
@property (nonatomic) DTX_DataCollectionLevel dataCollectionLevel;
@end


/*!
 @brief Provides for startup and shutdown of the Dynatrace OneAgent.

 The Dynatrace class provides the ability to startup, shutdown, and control other global
 attributes of the Dynatrace OneAgent.
 */
@interface Dynatrace : NSObject

/*!
 @brief Initializes the Dynatrace OneAgent. This is deprecated! Please use (DTX_StatusCode)startupWithConfig:(NSDictionary*)config instead.

 This must be invoked before any Events can be captured. Multiple calls to this method are
 ignored if the OneAgent is not shut down between them.

 This method will be called automatically if you specify the value DTXAgentStartupPath in
 your application's Info.plist file. The parameter descriptions below specify the info.plist value
 to use to specify each parameter value when using auto-start.

 @param applicationName A user-defined name for the application. (Info.plist value
 DTXApplicationID)

 @param serverURL The URL of the web server with an active Dynatrace UEM agent
 (eg: "http://myhost.mydomain.com:8080/agentLocation/"). The URL scheme dictates whether the OneAgent
 uses http or https. The URL must contain the agent location specified in the Dynatrace UEM settings
 for this application. (Info.plist value DTXAgentStartupPath)

 @param allowAnyCert Allow any certificate for https communication. This is only evaluated if
 the https transport mechanism is specified in the server name. (Info.plist value
 DTXAllowAnyCert)

 @param pathToCertificateAsDER Path to a (self-signed) certificate in DER format or nil. Adds a
 certificiate in DER format which is used as an additional anchor to validate https communication.
 This is needed if allowAnyCert is NO and a self-signed certificate is used on the server. You can
 retrieve the path for a file in your application bundle using code like this:

 <tt>
 NSString *pathToCertificateAsDER =
     [[NSBundle mainBundle] pathForResource:\@"myAppCert" ofType:\@"der"];
 </tt>

 (Info.plist value DTXAgentCertificatePath)

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)startupWithApplicationName:(NONNULL NSString*)applicationName
                                   serverURL:(NONNULL NSString*)serverURL
                                allowAnyCert:(BOOL)allowAnyCert
                             certificatePath:(NULLABLE NSString*)pathToCertificateAsDER __deprecated_msg("Please use (DTX_StatusCode)startupWithConfig:(NSDictionary*)config instead");

/*!
 @brief Initializes the Dynatrace OneAgent.

 This must be invoked before any Events can be captured. Multiple calls to this method are
 ignored if the OneAgent is not shut down between them.

 This method will be called automatically if you specify the value DTXAgentStartupPath in
 your application's info.plist file. This method has no paramaters. It uses the paramaters specified in info.plist.


 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)startupWithInfoPlistSettings;

/*!
 @brief Initializes the Dynatrace OneAgent.
 
 @param config A dictionary with key/value pairs similar to the ones in the project's Info.plist file.
 
 @discussion An overview of all possible key/value pairs can be found on the start page under "Constants"
 
 @return Returns a DTX_StatusCode
 */

+ (DTX_StatusCode)startupWithConfig:(NONNULL NSDictionary<NSString*,id>*)config;

/*!
 @brief Stops Dynatrace monitoring and flushes all collected data to the server.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)shutdown;

/*!
 @brief Identify a user.

  The current session will be tagged with the provided userId. The userId needs to be set again on application restart.

  @param userId to tag the session with. If userId = nil or empty the user tag will be removed from the session.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)identifyUser:(NULLABLE NSString*)userId;

/*!
 @brief Identify a user.

 The current session will be tagged with the provided userId. The userId needs to be set again on application restart.

 @param userId to tag the session with. If userId = nil or empty the user tag will be removed from the session.

  @param parentAction The parent action for the user identify.


 @return Returns a DTX_StatusCode
 */

+ (DTX_StatusCode)identifyUser:(NULLABLE NSString*)userId parentAction:(NULLABLE DTXAction*)parentAction;
/*!
 @brief Set the current GPS location of the user.

 The Dynatrace library does not automatically collect location information. If the
 developer wants location information to be transmitted to Dynatrace, then this function should
 be used to provide it.

 @param gpsLocation CLLocation object with GPS coordinates aquired by customer application

 @return Returns a DTX_StatusCode indicating current uem capture status (if the OneAgent is not
 capturing no GPS location is set)
 */
+ (DTX_StatusCode)setGpsLocation:(NONNULL CLLocation*)gpsLocation;

/*!
 @brief Provides information regarding internal errors.

 Use this to obtain the error code associated with the most recent DTX_Error_InternalError or
 enterActionWithName.

 @return Returns the error code or 0 if there is no error.
 */
+ (int)lastErrorCode;

/*!
 @brief Provides a string describing internal errors.

 Use this to obtain the error message associated with most recent DTX_Error_InternalError.

 @return Returns the error message or nil if there is no error.
 */
+ (NULLABLE NSString*)lastErrorMsg;

/*!
 @brief Send all collected events immediately.

 To reduce network traffic/usage the collected events are usually sent in packages where the oldest
 event has an age of up to 9 minutes. Using this method you can force sending of all collected
 events regardless of their age.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)flushEvents;

/*!
 @brief Enable Dynatrace crash reporting.

 The Dynatrace OneAgent can report on application crashes using the KSCrash framework. Call this
 method after startup to enable crash reporting to the Dynatrace server.

 When using auto-start use the Info.plist value DTXCrashReportingEnabled to control whether this
 method is invoked automatically. You must disable the automatic invocation of this method if
 you wish to use one of the following methods to enable third party crash reporting.

 @param sendCrashReport YES to send complete crash report to Dynatrace server. NO to
 send only minimal information.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)enableCrashReportingWithReport:(BOOL)sendCrashReport;

/*!
 @brief Enable Dynatrace and HockeyApp crash reporting.

 The Dynatrace OneAgent can report on application crashes using the KSCrash framework. Call this
 method after startup to enable crash reporting to the Dynatrace server as well as HockeyApp.
 Crash information will be sent to both systems.

 When using auto-start you must set the DTXCrashReportingEnabled info.plist value to NO when using
 this method to initiate crash reporting.

 @param sendCrashReport YES to send complete crash report to Dynatrace server. NO to
 send only minimal information.

 @param appIdentifier The application identifier assigned by HockeyApp to your application.

 @param waitUntilReachable YES to wait until HockeyApp is reachable before sending. NO to attempt
 sending whether available or not and simply fails if not available.

 @param userId Optional HockeyApp user ID. Set to nil if not needed.

 @param userName Optional HockeyApp user name. Set to nil if not needed.

 @param contactEmail Optional HockeyApp user contact email address. Set to nil if not needed.

 @param description Optional description to include with the crash report for HockeyApp.

 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)enableCrashReportingWithReport:(BOOL)sendCrashReport
                             hockeyAppIdentifier:(NONNULL NSString*)appIdentifier
                        hockeyWaitUntilReachable:(BOOL)waitUntilReachable
                                    hockeyUserId:(NULLABLE NSString*)userId
                                  hockeyUserName:(NULLABLE NSString*)userName
                              hockeyContactEmail:(NULLABLE NSString*)contactEmail
                          hockeyCrashDescription:(NULLABLE NSString*)description;

/*!
 @brief Enable Dynatrace and email crash reporting.

 The Dynatrace OneAgent can report on application crashes using the KSCrash framework. Call this
 method after startup to enable crash reporting to the Dynatrace server as well as by email.
 Crash reports will be sent to the Dynatrace server, and the device user will have the opportunity
 to send the report by email to the address(es) of your choice.

 The device user will first see an alert on their screen. You control the title, text, and button
 labels on the alert. If the user taps the yes button, then the email UI will popup on the
 screen. It will be preloaded with a message containing the recipients, subject, message, and the
 crash report as an attachment. The can modify the message before sending it.

 When using auto-start you must set the DTXCrashReportingEnabled info.plist value to NO when using
 this method to initiate crash reporting.

 @param sendCrashReport YES to send complete crash report to Dynatrace server. NO to
 send only minimal information.

 @param recipients An array of NSString's each containing one email address.

 @param subject The email subject.

 @param message The email body text.

 @param filename The name to use for the file attachment containing the crash report. Set to
 nil to use the default name.

 @param sendAppleStyleReport YES to send an Apple style crash report. NO to send a JSON style
 crash report.

 @param alertTitle Title for the alert.

 @param alertMessage Message for the alert.

 @param yesButtonText Label for the yes button in the alert. If the user taps this button, then
 the email UI appears.

 @param noButtonText Label for the no button in the alert. If the user taps this button, then
 the crash report is not emailed. It is still sent to Dynatrace.

 @return Returns a DTX_StatusCode
 */

+ (DTX_StatusCode)enableCrashReportingWithReport:(BOOL)sendCrashReport
                                 emailRecipients:(NULLABLE NSArray*)recipients
                                    emailSubject:(NULLABLE NSString*)subject
                                    emailMessage:(NULLABLE NSString*)message
                                   emailFilename:(NULLABLE NSString*)filename
                            sendAppleStyleReport:(BOOL)sendAppleStyleReport
                                      alertTitle:(NULLABLE NSString*)alertTitle
                                    alertMessage:(NULLABLE NSString*)alertMessage
                                   yesButtonText:(NULLABLE NSString*)yesButtonText
                                    noButtonText:(NULLABLE NSString*)noButtonText;

/*!
 @brief Sets a dictionary of custom headers to be included in all OneAgent data transmissions.
 
 The Dynatrace OneAgent sends data to the server via HTTP or HTTPS. If your infrastructure requires
 custom headers to be added to HTTP requests in order for them to pass you can use this method to provide
 them. This method needs to be called as early as possible to ensure that the headers are available for the first communication with the server.
 It can be called again later to change or to remove the headers.

 @param beaconHeaders The headers dictionary. Pass nil to remove all headers from future requests.
 The headers will be inspected for invalid or not allowed values.
     Limitations:
     - max number of headers: 10
     - max header len: 4096 bytes
     - max total size: 8192 bytes
 
 @return YES if the headers where successfully set, NO otherwise.
 */
+ (BOOL)setBeaconHeaders:(NULLABLE NSDictionary<NSString*,NSString*>*)beaconHeaders;

/**
 @return the dictionary of custom headers that was set with setBeaconHeaders: method
 */
+ (NULLABLE NSDictionary<NSString*,NSString*>*)beaconHeaders;

/**
 In order to receive notifications about the network errors occurred in the agent
 subscribe to the notification center using this as name.
 
 e.g.: [[NSNotificationCenter defaultCenter] addObserverForName:[Dynatrace getCommunicationProblemNotificationName] ... ];
 */
+ (NONNULL NSString *)getCommunicationProblemNotificationName;

/*!
 @brief Returns the HTTP header to use for manual web request tagging.

 The Dynatrace OneAgent automatically tags web requests made using NSURLRequest, NSURLConnection,
 NSURLProtocol and NSString. If you use an alternate technology to make
 web requests and want them to appear nested in a mobile action PurePath use this method and the
 getRequestTagValue method to retrieve the HTTP header and its value. Adding that information to
 your request will include it in the PurePath.

 @return Returns the name of the HTTP header that you must add to your web request.
 */
+ (NONNULL NSString*)getRequestTagHeader;

/*!
 @brief Returns the HTTP header value to use for manual web request tagging.

 The Dynatrace OneAgent automatically tags web requests made using NSURLRequest, NSURLConnection,
 NSURLProtocol and NSString. If you use an alternate technology to make
 web requests and want them to appear nested in a mobile action PurePath use this method and the
 getRequestTagHeader method to retrieve the HTTP header and its value. Adding that information to
 your request will include it in the PurePath.

 @param url The NSURL for the web request.

 @return Returns the value of the HTTP header that you must add to your web request.
 */
+ (NULLABLE NSString*)getRequestTagValueForURL:(NULLABLE NSURL*)url;

/*!
 @brief Returns the current data collection level. This method will always return DTX_DataCollectionUserBehavior when the user optin feature is not used.
 
 @return current data collection level
 */
+ (DTX_DataCollectionLevel)dataCollectionLevel __deprecated_msg("use userPrivacyOptions API");

/*!
 @brief Allows the user to set the data collection level and stores the users decisions for future sessions.
 
 This method does not store the settings when the user optin feature is not used.
 When the user changes the data collection level a new session will be started.

 @param dataCollectionLevel new data collection level
 
 @param completion callback that is executed when switching to the new data collection level is finished
 */
+ (void)setDataCollectionLevel:(DTX_DataCollectionLevel)dataCollectionLevel completion:(void(^ _Nullable)(BOOL successful))completion __deprecated_msg("use userPrivacyOptions API");

/*!
 @brief Returns the current privacy setting for crash reporting. This method will always return YES, when the user optin feature is not used.
 
 @return current privacy setting for crash reporting
 */
+ (BOOL)crashReportingOptedIn __deprecated_msg("use userPrivacyOptions API");

/*!
 @brief Allows the user to activate/deactivate crash reporting and stores the users decisions for future sessions.
 
 This method does not store the settings when the user optin feature is not used.
 @param optedIn YES, if crash reporting is confirmed by the user
 */
+ (void)setCrashReportingOptedIn:(BOOL)optedIn __deprecated_msg("use userPrivacyOptions API");

/*!
 @brief Ends the visit for Dynatrace and starts a new session.
 
 This method creates its own mobile-only PurePath.
 
 @return Returns a DTX_StatusCode
 */
+ (DTX_StatusCode)endVisit;

/*!
@brief Returns an object containig privacy options that can be made available to the application user.
 
When the user optin feature is not used:
    - all the BOOL options default to YES.
    - dataCollection level defaults to: DTX_DataCollectionUserBehavior
*/
+ (id<DTXUserPrivacyOptions> _Nonnull)userPrivacyOptions;

/*!
@brief Allows the user to activate/deactivate various privacy options. This are detailed in the DTXUserPrivacyOptions protocol declaration
 
@param completion callback that is executed when applying  the new option is completed. (Can be ommited but its higly recommendable to be used. In best case scenario the application should wait for it.)
*/
+ (void)applyUserPrivacyOptions:(id<DTXUserPrivacyOptions> _Nonnull)userOptions completion:(void(^ _Nullable)(BOOL successful))completion;

@end
#endif
/*!
 @const kDTXLogLevel
 If this key is present with a valid value (ALL, FINEST, FINER, FINE, CONFIG, INFO, WARNING, SEVERE, OFF),
 the OneAgent logging level will be set to this value. If a key isn't present or doesn't have a valid value,
 logging will default to INFO.
 */
extern NSString *_Nonnull const kDTXLogLevel;

/*!
 @const kDTXApplicationID
 This key’s value is used by Dynatrace to identify your mobile app. Auto-instrumentation issues an error if the key isn't present.
 */
extern NSString *_Nonnull const kDTXApplicationID;

/*!
 @const kDTXAgentCertificatePath
 This key defines the path to a (self-signed) certificate in DERformat, which is used as an additional anchor
 to validate HTTPS communication. This key is needed if DTXAllowAnyCert is false and a self-signed certificate
 is used on the server. The default value is null.
 */
extern NSString *_Nonnull const kDTXAgentCertificatePath;

/*!
 @const kDTXInstrumentWebViewTiming
 Detects and times web requests that originate from web views in your app. Set the value to false to disable
 automatic detection and timing of web view web requests. The default value is true.
 */
extern NSString *_Nonnull const kDTXInstrumentWebViewTiming;

/*!
 @const kDTXExcludedControls
 Defines an array of items where each item contains a type of view or control to exclude from automatic creation
 of user actions. Each item in the array is a case-insensitive string. Possible values are Button, DatePicker, Slider, Stepper,
 Switch, RefreshControl, ToolBar, SegmentedControl, TableView, TabBar, AlertView, AlertAction, PageView, NavigationController,
 CollectionView, Gesture, and ActionSheet.
 */
extern NSString *_Nonnull const kDTXExcludedControls;

/*!
 @const kDTXExcludedControlClasses
 An array of items where each item contains the name of a UI control (sub-)class to exclude from automatic instrumentation.
 Each item in the array is a case-sensitive string that must exactly match the name of the class to be excluded.
 */
extern NSString *_Nonnull const kDTXExcludedControlClasses;

/*!
 @const kDTXExcludedLifecycleClasses
 An array of items where each item contains the name of a class to exclude from automatic lifecycle instrumentation.
 Each item in the array is a case-sensitive string that must exactly match the name of the class to be excluded.
 */
extern NSString *_Nonnull const kDTXExcludedLifecycleClasses;

/*!
 @const kDTXCrashReportingEnabled
 Set the value to false if you want to disable crash reporting. The default value is true.
 */
extern NSString *_Nonnull const kDTXCrashReportingEnabled;

/*!
 @const kDTXSendCrashReports
 Corresponds to the sendCrashReport parameter for enableCrashReportingwithReport to send crash reports to the AppMon Server.
 Set the key to false if you do not want to send crash reports. The default value is true.
 */
extern NSString *_Nonnull const kDTXSendCrashReports;

/*!
 @const kDTXInstrumentLifecycleMonitoring
 Automatic lifecycle detection is enabled without the need to override your view controller classes with the OneAgent
 for iOS lifecycle classes. Set the value to false to disable automatic lifecycle monitoring. The default value is true.
 */
extern NSString *_Nonnull const kDTXInstrumentLifecycleMonitoring;

/*!
 @const kDTXInstrumentWebRequestTiming
 The default value is true, which turns on automatic web request timing. Set the value to false to disable automatic
 web request timing. Note that webRequestTagging is enabled if webRequestTiming is enabled, because it's a dependency.
 */
extern NSString *_Nonnull const kDTXInstrumentWebRequestTiming;

/*!
 @const kDTXInstrumentAutoUserAction
 The default value is true. Turns on the ability to automatically create user actions for user interactions with the app,
 such as button clicks. Set the value to false to disable automatic creation of user actions.
 */
extern NSString *_Nonnull const kDTXInstrumentAutoUserAction;

/*!
 @const kDTXAutoActionTimeoutMilliseconds
 The default value is 500 ms. Sets the value for how long a particular automatic user action is active.
 The purpose is to catch all web requests that happen when an automatic user action is active.
 If the automatic user action has completed web requests, OneAgent leaves the action at the end of this time.
 The minimum allowed value is 100 ms, the maximum allowed value is 5000 ms (five seconds).
 */
extern NSString *_Nonnull const kDTXAutoActionTimeoutMilliseconds;

/*!
 @const kDTXAutoActionMaxDurationMilliseconds
 The default value is 60000 ms (60 seconds). Sets the amount of time to retain an automatic user action before deletion.
 The purpose is to catch all web requests that occur when an automatic user action is active.
 If an automatic user action has pending web requests that are taking a long time to complete,
 OneAgent waits for this amount of time for the web requests to complete before leaving the user action.
 The minimum allowed value is 100 ms, the maximum allowed value is 540000 ms (nine minutes).
 */
extern NSString *_Nonnull const kDTXAutoActionMaxDurationMilliseconds;

/*!
 @const kDTXSendEmptyAutoAction
 Determines whether to send automatic user actions that don't contain any web requests or lifecycle actions.
 The default value is true.
 */
extern NSString *_Nonnull const kDTXSendEmptyAutoAction;

/*!
 @const kDTXInstrumentGPSLocation
 The location is captured only if the app uses CLLocationManager and sends the captured location as a metric to the server.
 OneAgent for iOS doesn't perform GPS location capturing on its own. Set the value to false to disable OneAgent
 for iOS location capturing. The default value is true.
 */
extern NSString *_Nonnull const kDTXInstrumentGPSLocation;

/*!
 @const kDTXAllowAnyCert
 If set to true, self signed certificates are accepted.
 If set to false, valid certificates from known certificate authorities are accepted. The default value is false.
 */
extern NSString *_Nonnull const kDTXAllowAnyCert;

/*!
 @const kDTXHybridApplication
 The default value is false. Set the value to true if you have a Hybrid application.
 This is necessary to share the same visit for user actions created by the JavaScript agent.
 */
extern NSString *_Nonnull const kDTXHybridApplication;

/*!
 @const kDTXUserOptIn
 If set to true, privacy mode is activated and user consent needs to be queried and set.
 The privacy settings for data collection and crash reporting can be changed via the OneAgent SDK for Mobile like described in Data privacy.
 The default value is false.
 https://www.dynatrace.com/support/help/user-experience/mobile-apps/how-do-i-enrich-mobile-user-experience-data-via-the-oneagent-sdk-for-mobile/#anchor_privacy
 */
extern NSString *_Nonnull const kDTXUserOptIn;

/*!
 @const kDTXAgentStartupPath (Appmon)
 If this key is present with a valid value, the Mobile Agent starts automatically and using this value as the serverUrl name.
 It ignores any manual startup calls in your app. serverUrl must start with the transport mechanism to use (http:// or https://). 
 */
extern NSString *_Nonnull const kDTXAgentStartupPath;

/*!
 @const kDTXBeaconURL (Dynatrace)
 The beacon URL as specified by the mobile application settings in the Dynatrace webUI.
 This value needs to begin with the transport mechanism to be used (http:// or https://).
 */
extern NSString *_Nonnull const kDTXBeaconURL;

/*!
@const kDTXInstrumentFrameworks
 Flag to force the agent to also look for viewControllers inside the frameworks linked with the application
*/
extern NSString *_Nonnull const kDTXInstrumentFrameworks;
