//
//  ChatViewController.m
//  ChatDemo
//
//  Created by bytedance on 2020/8/25.
//  Copyright © 2020 sjtu. All rights reserved.
//

#import "ChatViewController.h"

#define ScreenHeight [UIScreen mainScreen].bounds.size.height

@interface ChatViewController () <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, copy) NSString *user;

@property (nonatomic, weak) UIView *inputContainer;

@property (nonatomic, weak) UITableView *messageTable;

@property (nonatomic, weak) UITextField *input;

@property (nonatomic, strong) NSMutableArray *messageArray;

@end

@implementation ChatViewController

- (id)initWithRoom:(NSString *)room user:(NSString *)user {
    self = [super init];
    if (self) {
        self.title = [NSString stringWithFormat:@"房间: %@", room];
        self.user = user;
        self.messageArray = [NSMutableArray array];
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    CGFloat inputContainerHeight = 60;
    CGFloat naviHeight = self.navigationController.navigationBar.frame.size.height;
    
    UITableView *messageTable = [[UITableView alloc] initWithFrame:CGRectMake(0, naviHeight, self.view.frame.size.width, self.view.frame.size.height - inputContainerHeight - naviHeight) style:UITableViewStylePlain];
    messageTable.separatorStyle = UITableViewCellSeparatorStyleNone;
    messageTable.delegate = self;
    messageTable.dataSource = self;
    [self.view addSubview:messageTable];
    [messageTable registerClass:[UITableViewCell class] forCellReuseIdentifier:@"cell"];
    _messageTable = messageTable;
    
    UIView *inputContainer = [[UIView alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - inputContainerHeight, self.view.frame.size.width, inputContainerHeight)];
    inputContainer.backgroundColor = [UIColor colorWithRed:0 green:0.67 blue:1.0 alpha:1.0];
    [self.view addSubview:inputContainer];
    _inputContainer = inputContainer;
    
    UITextField *input = [[UITextField alloc] init];
    input.frame = CGRectMake(10, 10, inputContainer.frame.size.width - 20 - 50, 40);
    input.borderStyle = UITextBorderStyleRoundedRect;
    [inputContainer addSubview:input];
    _input = input;
    
    UIButton *sendButton = [UIButton buttonWithType:UIButtonTypeCustom];
    sendButton.frame = CGRectMake(inputContainer.frame.size.width - 50, 10, 40, 40);
    [sendButton setTitle:@"发送" forState:UIControlStateNormal];
    [sendButton addTarget:self action:@selector(sendButtonTapped:) forControlEvents:UIControlEventTouchUpInside];
    [inputContainer addSubview:sendButton];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardChanged:) name:UIKeyboardWillChangeFrameNotification object:nil];
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapRecognized)];
    [self.view addGestureRecognizer:tapGesture];
}

- (void)keyboardChanged:(NSNotification *)notification {
    CGRect keyboardFrame = [notification.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    CGRect currentFrame = _inputContainer.frame;
    [UIView animateWithDuration:0.25 animations:^{
        if (keyboardFrame.origin.y == ScreenHeight) {
            self.inputContainer.frame = CGRectMake(currentFrame.origin.x,
                                                   ScreenHeight - currentFrame.size.height,
                                                   currentFrame.size.width,
                                                   currentFrame.size.height);
        } else {
            self.inputContainer.frame = CGRectMake(currentFrame.origin.x,
                                                   ScreenHeight - currentFrame.size.height - keyboardFrame.size.height,
                                                   currentFrame.size.width,
                                                   currentFrame.size.height);
        }
    }];
}

- (void)tapRecognized {
    [self.view endEditing:YES];
}

- (void)sendButtonTapped:(id)sender {
    if (_input.text.length > 0) {
        [_messageArray addObject:_input.text];
    }
    
    [_messageTable reloadData];
    
    _input.text = @"";
}

#pragma mark UITableView Delegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return _messageArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    cell.textLabel.text = [NSString stringWithFormat:@"%@: %@", self.user, [_messageArray objectAtIndex:indexPath.row]];
    return cell;
}

- (nullable NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    return [NSString stringWithFormat:@"Welcome %@", self.user];
}

@end
