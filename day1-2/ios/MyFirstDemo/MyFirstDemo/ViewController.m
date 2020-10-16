//
//  ViewController.m
//  MyFirstDemo
//
//  Created by bytedance on 2020/8/23.
//  Copyright © 2020 sjtu. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@property (nonatomic, strong) UILabel *label;

@property (nonatomic, strong) UITextField *textField;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    //新建一个label控件
    UILabel *label = [[UILabel alloc] init];
    
    //设置这个label控件的位置
    label.frame = CGRectMake(0, 0, 200, 100);
    
    label.center = self.view.center;
    
    //设置这个label控件的内容
    label.text = @"Hello World!";
    
    label.font = [UIFont systemFontOfSize:30];
    
    [label setTextColor:[UIColor blueColor]];
    
    //最后一步，把这个label控价加入到当前视图控制器的视图中
    [self.view addSubview:label];
    _label = label;
    
    UITextField *textField = [[UITextField alloc] init];
    textField.frame = CGRectMake(30, 50, 200, 50);
    textField.placeholder = @"Input your name here";
    [self.view addSubview:textField];
    _textField = textField;
    
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.frame = CGRectMake(240, 50, 100, 50);
    [button setTitle:@"click me" forState:UIControlStateNormal];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [button addTarget:self action:@selector(buttonTapped:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
}

- (void)buttonTapped:(id)sender {
    _label.text = [NSString stringWithFormat:@"Hello %@", _textField.text];
    [_label sizeToFit];
    _label.center = self.view.center;
}

@end
