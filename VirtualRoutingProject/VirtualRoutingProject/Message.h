#pragma once
#ifndef  _MESSAGE_H_
#define _MESSAGE_H_
#include "utils.h"
/*****************************************************

Author: qyh

Date created : 18/11/18

Date finished:

Description: 使用结构体封装了通信报文中的字段，集成了编码和解码
			 的功能，便于网络传输和接收数据。

****************************************************/


/*

大部分照搬里头链接源码，自定义应用层协议，
打包成网络字节流，以及将网络字节流解包成本地字符流
https://segmentfault.com/a/1190000008740863#articleHeader6

*/

/************************************************************************************
报文设计: 定长协议头 + 变长json协议体

协议头Header：除了server字段，其他无需修改
	server字段有2种类型:
	0  代表 RIP
	1  代表 OSPF

	注：当源节点发送报文经过多个路由时，每发送一个路由，该路由就要发送回显报文，告知源节点，消息传输情况。
	    返回路径仍旧需要经过多个路由，而不是直接发送给源节点。
	
协议体body： 
	json有几个数据段：
	报文类型 type  (int)：

	1  代表从源节点开始发送的消息报文  （消息要经过多个路由）
	2  代表转发源节点的消息报文
	3  代表节点之间交换路由信息的报文  （消息点对点）

	针对上述三种报文又有以下不同的字段
	1 2 Src_Addr: 报文发送源地址  Dst_Addr: 报文发送目的地址 Next_Addr: 下一跳地址 From_Addr: 本发送节点地址  Msg: 消息 Extra_Data:多余的消息
	3   Src_Addr: 报文发送源地址  Dst_Addr: 报文发送目的地址  Msg: 交换的路由信息 Extra_Data: 多余的消息，可为空

	Msg:
	1 2  消息报文里的内容直接是 string 类型 代表消息
	3 交换报文内的内容是 数组。如果是RIP则是一维数组，如果是OSPF就是二维数组。 
	
***************************************************************************************/

/*
	MyProto打包类
 */
class MyProtoEnCode
{
public:
	//协议消息体打包函数
	uint8_t * encode(MyProtoMsg * pMsg, uint32_t & len);
private:
	//协议头打包函数
	void headEncode(uint8_t * pData, MyProtoMsg * pMsg);
};


/*
	MyProto解包类
 */
class MyProtoDeCode
{
public:
	void init();    
	void clear();
	bool parser(void * data, size_t len);
	bool empty();
	MyProtoMsg * front();
	void pop();
private:
	bool parserHead(uint8_t ** curData, uint32_t & curLen,
		uint32_t & parserLen, bool & parserBreak);
	bool parserBody(uint8_t ** curData, uint32_t & curLen,
		uint32_t & parserLen, bool & parserBreak);

private:
	MyProtoMsg mCurMsg;                     //当前解析中的协议消息体
	queue<MyProtoMsg *> mMsgQ;              //解析好的协议消息队列
	vector<uint8_t> mCurReserved;           //未解析的网络字节流
	MyProtoParserStatus mCurParserStatus;   //当前解析状态
};

/*将打包和解包，以及生成报文集成到一个类中*/
class Message
{
private:
	Message();                             // ctor hidden
	Message(Message const&);               // copy ctor hidden
	Message& operator=(Message const&);    // assign op. hidden
	~Message();                            // dtor hidden
	MyProtoDeCode decoder;             /*解码器*/
	MyProtoEnCode encoder;             /*编码器*/

public:
	// 单例模式
	static Message& Instance() {
		static Message theMessage;  /*使用方法 Message::Instance()得到对象*/
		return theMessage;
	}

	//TODO  需要修改输入参数，返回值
	
	/*生成一份报文*/
	void CreateMessage();

	/*文本字符流转为网络字节流，便于传输*/
	void EncodeMessage();

	/*将网络字节流转为文本字符*/
	void DecodeMessage();

};

#endif // ! _MESSAGE_H_
