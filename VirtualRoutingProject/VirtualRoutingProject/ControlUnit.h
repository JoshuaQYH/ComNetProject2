#pragma once
#ifndef  _CONTROL_UNIT_H_
#define _CONTROL_UNIT_H_

/*****************************************************

Author: qyh

Date created : 18/11/19

Date finished:

Description: 总控程序，作用：控制通信，协议解析，以及路由选择

****************************************************/


class ControlUnit {

private:
	ControlUnit();                            // ctor hidden
	ControlUnit(ControlUnit const&);            // copy ctor hidden
	ControlUnit& operator=(ControlUnit const&); // assign op. hidden
	~ControlUnit();                           // dtor hidden

public:
	static ControlUnit& Instance() {
		static ControlUnit theControlUnit;
		return theControlUnit;
	}

	/* more (non-static) functions here */

};


#endif // ! _CONTROL_UNIT_H_