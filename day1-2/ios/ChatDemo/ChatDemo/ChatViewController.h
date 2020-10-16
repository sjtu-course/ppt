//
//  ChatViewController.h
//  ChatDemo
//
//  Created by bytedance on 2020/8/25.
//  Copyright © 2020 sjtu. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ChatViewController : UIViewController

- (id)initWithRoom:(NSString *)room user:(NSString *)user;

@end

NS_ASSUME_NONNULL_END
