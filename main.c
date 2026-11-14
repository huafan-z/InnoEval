#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "admin.h"
#include "judge.h"
int role;
int main() {
	if (!loadWorkFromFile()) {
		printf("System cannot start without data file.\n");
		go_on();
		return 1;// 不进系统
	}
	//adm_run();
	while (1) {
		role = chooserole();//身份选择
		int adm_pass = 0, jud_pass = 0;//adm或jud的成功登录标志
		switch (role) {
			case 1:adm_pass = adm_login();break;
			case 2:jud_pass = jud_login();break;
		}
		if (adm_pass == 1 && role == 1) {//登录成功进入adm主界面
			adm_run();
		}
		else if (jud_pass == 1 && role == 2) {//登录成功进入jud主界面
			jud_run();
		}
		else continue;
		//是否再次进入system
		int is_againnum = is_again();
		if (is_againnum) continue;
		else break;
	}
	return 0;
}