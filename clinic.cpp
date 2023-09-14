//proje
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<Windows.h> 
#include<time.h>
#include<string.h>
#pragma execution_character_set("utf-8")
#pragma warning(disable:4996)
#define coll system("color B0");system("cls");
#define co1 system("color B0");
#define co2 system("cls");printtime();

//b1. b5.	be 	bd	 b8	  b9
struct date {
	unsigned day : 7;
	unsigned month : 5;
	unsigned year : 14;
	unsigned weekday : 4;  //sat:0 sun:1 mon:2...friday:6

}today;
struct dateandtime {
	date day;
	unsigned hour : 8;
	unsigned min : 8;
	unsigned sec : 8;
};
struct dateplus {
	date monthday;
	unsigned tatil : 1;	  //1 means closed
	char reason[50];
};
struct callender {
	dateplus days[50];
	int numbdays;
};
struct previsit {
	int patID;			 //if=0 means the time is free	   //if=-1 means no visit time
	dateandtime booktime;
	unsigned starthourvis : 6;
	unsigned startminvis : 7;
	unsigned endhourvis : 6;
	unsigned endminvis : 7;
	unsigned prescrpt : 2;
};
struct visits {
	date day;
	unsigned numberofvisits : 10;
	unsigned starthour : 7;
	unsigned endhour : 7;
	previsit v[160];
	int docID;
	char docname[20];
};
struct people {
	char name[20];		 //must not repit
	char name2pat[20];
	char codmeli[20];
	char email[20];
	char password[20];	 //only this can change
	int kind;	 //0admin 1doc 2 pat
	int ID;
	int wallet;
	int themonth;
	//saved in one file named people

};
struct doctor {
	date startday;
	int payment;
	int themonth;
	int rentmode;  //(rent of last month) 0->first month  1->hasnt defined yt 2->extension(pay it next month) 3->paid 

	int durationifvis;
	int numbviss;
	visits viss[20];
	//...???
};
struct onevisit {
	people doc;				//docID		docname		patientID		appointment time & date		    time of reserve
	people pat;											    	  //sun 29.2.401   8:00 to 8:30		28.2.401 8:21:13
	dateandtime starth;
	dateandtime endh;
	dateandtime booktime;
};
struct prescription {
	char p[100];
	dateandtime time;
	int docid;
	char docname[20];
};
struct patient {
	int countprescrpt;
	prescription prescrpt[15];
};
int lastID;

dateandtime gettime();
void weektostring(int, char[]);
void monthtostring(int, char[]);
void printtime();
void printbigletters(char*, int);

void loading(int);
void updatepeoplefile(people[]);
void getpassword(char[]);
people signin(people[], int);
people forget(people[], int);


int admin(people[], int);
int doctor(people[], people);
int patient(people[], people);

void adddoc(int, people[]);
void addpat(int);

void printmonth(callender);
void monthlyschedual();
void visitschedual(people list[]);

void makevisitready(visits*, int);
void shift(int*, visits[], int*, int, char[]);
void showdocvisits(int, visits[], int, int, people[], int, int, int);

void book(people[], int);
void printprescrpt(int);
void addprescrpt(people[], int, int, int);
int rent(int, int*);


int main()
{
	int p = 0;
	coll;
	SetConsoleOutputCP(65001);
	loading(0);
	printf("Please enter today date: \n   Day: ");
	char s[10];
	gets_s(s);
	today.day = atoi(s);
	printf("   Month: ");
	gets_s(s);
	today.month = atoi(s);
	printf("   Year: ");
	gets_s(s);
	today.year = atoi(s);
	today.weekday = 0;
	if (today.year >= 1000) today.year -= 1000;
	char tm[25];
	monthtostring(today.month, tm);
	char ty[10];
	itoa(today.year, ty, 10);
	strcat(tm, ty);
	strcat(tm, ".bin");

	char ch[] = "CLINIC";
	char* CH = ch;



	//loading(0);
	while (1)
	{


		lastID = 1; //number of people ever
		FILE* fp1 = fopen("lastID.bin", "rb");
		if (fp1 == NULL) {
			fp1 = fopen("lastID.bin", "wb");
			if (fp1 == NULL) {
				printf("FAILED!!");
				return 0;
			}
			fprintf(fp1, "%d", lastID);
			fclose(fp1);
		}
		else {
			fscanf(fp1, "%d", &lastID);
			fclose(fp1);
		}

		people peoplelist[100];
		people admin1 = { "","","Admin","Admin","Admin",0,0 };

		if (lastID == 1)
		{
			fp1 = fopen("people.bin", "wb");
			fwrite(&admin1, sizeof(people), 1, fp1);
			fclose(fp1);
		}

		fp1 = fopen("people.bin", "rb");
		fread(peoplelist, sizeof(people), lastID, fp1);
		fclose(fp1);




		coll;

		printf("\n");
		printbigletters(CH, 30);

		printf("\n1.Sign in\n2.Forgot password\n3.Exit\n");

		FILE* w = fopen(tm, "rb");
		if (!w) printf("   warning : this month hasnt been scheduled yet!\n");

		else {
			callender call;
			fread(&call, sizeof(call), 1, w);
			today.weekday = call.days[today.day].monthday.weekday;
		}

		//printf("lastId %d\n", lastID);
		char s[10];
		gets_s(s);
		int ch = atoi(s);
		people user;
		if (ch == 3)
			return 0;

		else if (ch == 1)
			user = signin(peoplelist, lastID);
		else if (ch == 2)
			user = forget(peoplelist, lastID);
		else continue;
		switch (user.kind)
		{
		case 0:
			loading(1);
			admin(peoplelist, lastID);
			break;
		case 1:
			loading(1);
			doctor(peoplelist, user);
			break;
		case 2:
			loading(1);
			patient(peoplelist, user);
			break;

		}

	}


	return 0;

}

void loading(int n)
{
	coll
		printf("\n\n\t\t please wait ...\n");
	printf("\n\t\033[107m");
	for (int i = 0; i < 31; i++)
		printf(" ");
	printf("\r\t");
	for (int i = 0; i < 31; i++) {
		printf("█");
		Sleep(40);
	}
	Sleep(300);
	coll
}
dateandtime gettime() {
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	dateandtime a;
	a.day = today;
	a.hour = timeinfo->tm_hour;
	a.min = timeinfo->tm_min;
	a.sec = timeinfo->tm_sec;
	return a;
}
void printtime() {
	char w[5];
	weektostring(today.weekday, w);
	dateandtime now = gettime();
	printf("Date : %s %d.%d.%d\tTime %d:%d\n", w, today.year, today.month, today.day, now.hour, now.min);
}

void getpassword(char name[])
{
	int i = 0;
	while (1) {
		name[i] = getch();
		if (name[i] == '\b') {
			name[i] = '\0';
			if (i == 0);
			else {
				i--;
				printf("\b \b");
				name[i] = '\0';
			}
		}
		else if (name[i] == '\r') {
			name[i] = '\0';
			break;
		}
		else {
			printf("*");
			i++;
		}
	}
}

people signin(people peoplelist[], int numb)
{
	char name[20];
	char password[20];
	people back;
	back.kind = -1;
	back.ID = -1;
	while (1) {

		coll;
		printf("username : ");
		gets_s(name);
		printf("password : ");
		getpassword(password);
		if (strcmp(name, "b") == 0 && strcmp(password, "b") == 0)
			return back;
		int x = 0;
		int i = 0;
		for (i = 0; i < numb; i++)
		{
			if (strcmp(name, peoplelist[i].codmeli) == 0)
			{
				x = 1;
				break;
			}
		}
		if (x == 0) {
			printf("\n\n\x01b[31mWrong username!");
			Sleep(550);
		}
		else {
			if (strcmp(password, peoplelist[i].password) == 0)
				return peoplelist[i];
			else {
				printf("\n\n\x01b[31mWrong password!");
				Sleep(550);
			}

		}

	}

	return back;
}
people forget(people peoplelist[], int numb)
{
	char name[20];
	char email[20];
	people back;
	back.kind = -1;
	back.ID = -1;
	while (1) {

		coll;
		printf("username : ");
		gets_s(name);
		printf("email : ");
		gets_s(email);
		if (strcmp(name, "b") == 0 && strcmp(email, "b") == 0)
			return back;
		int x = 0;
		int i = 0;
		for (i = 0; i < numb; i++)
		{
			if (strcmp(name, peoplelist[i].codmeli) == 0)
			{
				x = 1;
				break;
			}
		}
		if (x == 0) {
			printf("\n\x01b[31mWrong username!");
			Sleep(550);
		}
		else {
			if (strcmp(email, peoplelist[i].email) == 0)
			{
				while (1) {
					char pass1[20];
					char pass2[20];
					printf("\nEnter your new password : ");
					//scanf(" ");
					getpassword(pass1);
					printf("\nRe-Enter your new password : ");
					//scanf(" ");
					getpassword(pass2);
					if (strcmp(pass1, pass2) == 0) {
						strcpy(peoplelist[i].password, pass1);

						updatepeoplefile(peoplelist);
						return peoplelist[i];
					}
					else
						printf("\n\n\x01b[31mPasswords didn't match try again\n\x01b[30m");

				}


			}
			else {
				printf("\n\x01b[31mWrong email!");
				Sleep(550);
			}

		}

	}

	return back;
}

int admin(people peoplelist[], int numb)
{

	while (1) {
		coll;

		printtime();
		char ee[] = "ADMINnnnACOUNT";
		char* CH = ee;

		printf("\n");
		printbigletters(CH, 0);

		printf("\n");
		printf("1.Add Doctor\n2.Add Patient\n3.Monthly Schedule\n4.Visit Scedule\n5.Exit\n");
		char ch[10];
		gets_s(ch);
		int k = atoi(ch);

		switch (k) {
		case 1:
			adddoc(numb, peoplelist);
			numb++;
			break;
		case 2:
			addpat(numb);
			numb++;
			break;
		case 3:
			monthlyschedual();
			break;
		case 4:
			visitschedual(peoplelist);
			break;
		case 5:
			return 0;

		}
	}

	return 0;
}
int doctor(people peoplelist[], people user)
{

	char docfile[15];
	itoa(user.ID, docfile, 10);
	strcat(docfile, "doc.bin");
	FILE* fp = fopen(docfile, "rb");
	if (!fp) {
		printf("error reading file");
		char cc = getch();
		return 0;
	}
	struct doctor doc1;
	fread(&doc1, sizeof(doc1), 1, fp);
	fclose(fp);


	if (doc1.themonth != today.month) {

		//new month
		doc1.numbviss = 0;
		doc1.durationifvis = 0;
		doc1.payment = 0;
		doc1.themonth = today.month;
		if (doc1.rentmode == 0 || doc1.rentmode == 3) doc1.rentmode = 1;
		else if (doc1.rentmode == 2 || doc1.rentmode == 1) {
			//extended last month (or didnt work)
			doc1.rentmode = 4; //dangrous


		}
		else if (doc1.rentmode == 4) {
			printf("You got expelled! Good Bye\n");
			remove(docfile);
			strcpy(peoplelist[user.ID].name, "");
			strcpy(peoplelist[user.ID].codmeli, "");
			strcpy(peoplelist[user.ID].email, "");
			strcpy(peoplelist[user.ID].password, "");
			peoplelist[user.ID].kind = -1;
			updatepeoplefile(peoplelist);
			getch();
			return 0;
		}
	}


	while (1) {
		itoa(user.ID, docfile, 10);
		strcat(docfile, "doc.bin");

		fp = fopen(docfile, "wb");
		if (!fp) {
			printf("error reading file");
			char cc = getch();
			return 0;
		}
		fwrite(&doc1, sizeof(doc1), 1, fp);
		fclose(fp);

		coll;
		printtime();
		char ee[] = "DOCTORnnnACOUNT";
		char* CH = ee;

		printf("\n");
		printbigletters(CH, 0);

		printf("\n");
		printf("DoctorID : %d\nName : %s\nWallet : %d$\n\n", user.ID, user.name, peoplelist[user.ID].wallet);
		printf("1.Determining the shifts\n2.Reserved Visits\n3.Patient Prescription\n4.Rent Payment\n5.Visits Payment\n6.Exit\n");

		if (doc1.rentmode == 1 || doc1.rentmode == 4) {
			printf("   Warning: You must pay your rent or extend it\n");
		}
		if (doc1.durationifvis == 0) {
			printf("   Warning: You must declare duration of each visit\n");
		}
		if (doc1.numbviss == 0)  printf("   Warning: you must determine your shifts\n");
		if (doc1.payment == 0) printf("   Warning: You must determine the cost of each visit\n");

		char ch[10];
		gets_s(ch);
		int k = atoi(ch);
		if (doc1.rentmode == 1 || doc1.rentmode == 4) {
			if (k == 1) {
				printf("You cant determine your shifts unless you pay your rent or extend it\n");
				getchar();
				continue;
			}
		}


		switch (k) {
		case 1:
			shift(&doc1.numbviss, doc1.viss, &doc1.durationifvis, user.ID, user.name);
			break;
		case 2:
			showdocvisits(doc1.numbviss, doc1.viss, 1, 0, peoplelist, 0, 0, 0);
			break;
		case 3:
			showdocvisits(doc1.numbviss, doc1.viss, 1, 0, peoplelist, 2, user.ID, doc1.payment);
			break;
		case 4:
			printf("\x01b[35m");
			if (doc1.rentmode == 3 || doc1.rentmode == 0) {
				printf("No rent to pay :)\n");
				getch();
			}
			else if (doc1.rentmode == 2) {
				printf("You have extended for this month");
				getch();
			}
			else {
				doc1.rentmode = rent(doc1.rentmode, &peoplelist[user.ID].wallet);
				if (doc1.rentmode == 3)
					updatepeoplefile(peoplelist);
				else if (doc1.rentmode == 4 || doc1.rentmode == 5) {
					printf("You got expelled! Good Bye\n");
					remove(docfile);
					strcpy(peoplelist[user.ID].name, "");
					strcpy(peoplelist[user.ID].codmeli, "");
					strcpy(peoplelist[user.ID].email, "");
					strcpy(peoplelist[user.ID].password, "");
					peoplelist[user.ID].kind = -1;
					updatepeoplefile(peoplelist);
					getch();
					return 0;
				}
			}

			break;
		case 5:	 printf("\x01b[35m");
			if (doc1.payment > 0) {
				printf("Your payment each visit is %d$\nYou will recieve this when you write the patient prescription\nYou can change this the next month\n", doc1.payment);
				getch();
			}
			else {
				while (1) {
					printf("Enter the cost of each visit (between 10$ to 50$) : ");
					gets_s(ch);
					int cost = atoi(ch);
					if (cost >= 10 && cost <= 50) {
						doc1.payment = cost;
						printf("done\n");
						getch();
						break;
					}
					else {
						printf("the cost must be a number between 10$ to 50$\n");
					}
				}
				break;
			}
			break;
		case 6:
			return 0;
		}
	}

	return 0;
}
int patient(people peoplelist[], people user)
{
	if (user.themonth != today.month) {
		peoplelist[user.ID].themonth = today.month;
		updatepeoplefile(peoplelist);

		char patfile[15];
		itoa(user.ID, patfile, 10);
		strcat(patfile, "pat.bin");
		FILE* fp = fopen(patfile, "wb");
		if (!fp) {
			printf("error");
			char cc = getch();
			return 0;
		}
		struct patient pat1;
		pat1.countprescrpt = 0;
		fwrite(&pat1, sizeof(pat1), 1, fp);
		fclose(fp);
	}

	while (1) {
		coll;
		printtime();
		char ee[] = "PATIENTnnnACOUNT";
		char* CH = ee;

		printf("\n");
		printbigletters(CH, 0);

		printf("\n");
		printf("PatientID : %d\nName : %s\nWallet : %d$\n\n", user.ID, user.name2pat, peoplelist[user.ID].wallet);
		printf("1.Book an Appointment\n2.Cancel an Appointment\n3.All Appointments\n4.Prescriptions\n5.Charge my wallet\n6.Exit\n");
		char s[10];
		gets_s(s);
		int ch = atoi(s);
		int k = 0;
		visits temp[20];
		switch (ch) {
		case 1:
			book(peoplelist, user.ID);
			break;
		case 2:
			showdocvisits(0, temp, 3, user.ID, peoplelist, 1, 0, 0);//mode pluse on for canseling
			break;
		case 3:
			showdocvisits(0, temp, 3, user.ID, peoplelist, 0, 0, 0);
			break;
		case 4:
			printprescrpt(user.ID);
			break;
		case 5:
			printf("\x01b[34m");
			printf("how much do you want to charge your wallet? ");
			gets_s(s);
			k = atoi(s);
			if (k > 0) {
				user.wallet += k;
				peoplelist[user.ID].wallet += k;
				updatepeoplefile(peoplelist);
				printf("done\n");
				Sleep(800);
			}
			else {
				printf("Invalid");
				Sleep(800);
			}
			break;
		case 6:
			return 0;

		}
	}

	return 0;

}


void adddoc(int id, people list[]) {
	printf("\n\x01b[35m");
	struct doctor doc;
	struct people adddoc;
	printf("Adding new doctor ...\nID is %d\nName : ", id);

	while (1) {
		char temp[20];
		gets_s(temp);
		int f = 0;										   //????
		for (int i = 0;i < id;i++) {
			if (strcmp(temp, list[i].name) == 0) {
				f = 1;
				break;
			}
		}
		if (f == 1) printf("a doctor with this name already exist try another name\n");
		else {

			strcpy(adddoc.name, temp);
			break;
		}

		//check if repited	 
	}
	printf("Codemeli : ");
	gets_s(adddoc.codmeli);
	printf("Email : ");
	gets_s(adddoc.email);
	adddoc.ID = id;
	adddoc.themonth = today.month;
	doc.startday = today;
	doc.durationifvis = 0;
	doc.numbviss = 0;
	doc.payment = 0;
	doc.themonth = today.month;
	doc.rentmode = 0;
	adddoc.ID = id;
	adddoc.kind = 1;

	strcpy(adddoc.password, adddoc.email);
	adddoc.wallet = 10;

	printf("\x01b[95m\ndoctor added seccesfully\n");
	Sleep(800);
	char s[15];
	itoa(id, s, 10);
	strcat(s, "doc.bin");
	FILE* fp = fopen(s, "wb");
	if (!fp) {
		printf("error!\n");
		return;
	}
	fwrite(&doc, sizeof(struct doctor), 1, fp);
	fclose(fp);

	fp = fopen("lastID.bin", "wb");
	if (!fp) {
		printf("error!\n");
		return;
	}
	id++;
	fprintf(fp, "%d", id);
	fclose(fp);


	fp = fopen("people.bin", "ab");
	if (!fp) {
		printf("error!\n");
		return;
	}
	fwrite(&adddoc, sizeof(adddoc), 1, fp);
	fclose(fp);

	fp = fopen("people.bin", "rb");
	if (!fp) {
		printf("error!\n");
		return;
	}
	fread(list, sizeof(adddoc), id, fp);
	fclose(fp);


}
void addpat(int id) {

	printf("\n\x01b[35m");
	struct patient pat;
	struct people addpat;
	printf("Adding new patient ...\nID is %d\nName : ", id);
	strcpy(addpat.name, "");
	gets_s(addpat.name2pat);
	printf("Codemeli : ");
	gets_s(addpat.codmeli);
	printf("Email : ");
	gets_s(addpat.email);
	addpat.ID = id;
	pat.countprescrpt = 0;
	addpat.kind = 2;
	addpat.wallet = 10;
	addpat.themonth = today.month;
	strcpy(addpat.password, addpat.email);

	printf("\x01b[95m\nPatient added seccesfully\n");
	Sleep(800);
	char s[15];
	itoa(id, s, 10);
	strcat(s, "pat.bin");
	FILE* fp = fopen(s, "wb");
	if (!fp) {
		printf("error!\n");
		return;
	}
	fwrite(&pat, sizeof(struct patient), 1, fp);
	fclose(fp);

	fp = fopen("lastID.bin", "wb");
	if (!fp) {
		printf("error!\n");
		return;
	}
	id++;
	fprintf(fp, "%d", id);
	fclose(fp);


	fp = fopen("people.bin", "ab");
	if (!fp) {
		printf("error!\n");
		return;
	}
	fwrite(&addpat, sizeof(addpat), 1, fp);
	fclose(fp);
}

void monthtostring(int month, char st[]) {
	switch (month) {
	case 1:
		strcpy(st, "Farvardin");
		break;
	case 2:
		strcpy(st, "Ordibehesht");
		break;
	case 3:
		strcpy(st, "khordad");
		break;
	case 4:
		strcpy(st, "Tir");
		break;
	case 5:
		strcpy(st, "Mordad");
		break;
	case 6:
		strcpy(st, "Shahrivar");
		break;
	case 7:
		strcpy(st, "Mehr");
		break;
	case 8:
		strcpy(st, "Aban");
		break;
	case 9:
		strcpy(st, "Azar");
		break;
	case 10:
		strcpy(st, "Dey");
		break;
	case 11:
		strcpy(st, "Bahman");
		break;
	case 12:
		strcpy(st, "Esfand");
		break;

	}

}

void monthlyschedual() {
	printf("\n\x01b[34m");
	int monthh = today.month;
	char themonth[25];
	monthtostring(monthh, themonth);
	char year1[10];
	itoa(today.year, year1, 10);
	strcat(themonth, year1);
	strcat(themonth, ".bin");
	//check if exist already
	int existance = 1;
	FILE* fp = fopen(themonth, "rb");
	if (!fp) {
		existance = 0;
	}
	else
		fclose(fp);
	if (existance == 1) {
		while (1) {
			printf("1. Show this month schedule\n2. Create next months schedule\n3. Back\n");
			char s[10];
			gets_s(s);
			int a = atoi(s);
			if (a == 1) {
				//show .................
				fp = fopen(themonth, "rb");
				callender scheduled;
				fread(&scheduled, sizeof(callender), 1, fp);
				printmonth(scheduled);
				fclose(fp);
				char cc = getch();
				return;
			}
			else if (a == 2)
				monthh++;

			else if (a == 3) return;
			else continue;
			break;
		}


	}
	int a;
	while (1) {
		printf("Choose the month\n");
		int k = 0;
		for (int i = monthh;i <= 13;i++) {
			k++;
			if (i == 13) {
				printf("%d. Farvardin %d\n", k, today.year + 1);
			}
			else {
				char m[15];
				monthtostring(i, m);
				printf("%d. %s \n", k, m);
			}
			if (k == 4) break;
		}
		k++;
		printf("%d. Back\n", k);
		char s[10];
		gets_s(s);
		a = atoi(s);

		if (a == k) return;
		else if (a >= 1 && a < k);
		else {
			printf("Wrong input try again\n");
			continue;
		}
		a = a + monthh - 1;

		break;
	}
	char temp[25];
	int tmonth;
	int tyear;
	if (a == 13) {
		strcpy(themonth, "Farvardin");
		strcpy(temp, themonth);
		strcat(temp, " ");
		itoa(today.year + 1, year1, 10);
		strcat(themonth, year1);
		strcat(temp, year1);
		strcat(themonth, ".bin");
		tmonth = 1;
		tyear = today.year + 1;
	}
	else {
		monthtostring(a, themonth);
		strcpy(temp, themonth);
		strcat(temp, " ");
		itoa(today.year, year1, 10);
		strcat(themonth, year1);
		strcat(temp, year1);
		strcat(themonth, ".bin");
		tmonth = a;
		tyear = today.year;
	}

	FILE* fpp = fopen(themonth, "rb");
	if (fpp) {
		printf("This month has been escheduled befor\n1. Show the scedule      2. Back to menu\n");
		char s[10];
		gets_s(s);
		int g = atoi(s);
		if (g == 1) {
			//show .........................................................
			fp = fopen(themonth, "rb");
			callender scheduled;
			fread(&scheduled, sizeof(callender), 1, fp);
			printmonth(scheduled);
			char cc = getch();
			fclose(fpp);
			return;
		}

		else {
			fclose(fpp);
			return;
		}
	}

	co2;
	printf("\x01b[34m");
	callender newmonth;
	printf("%s\n\nEnter the number of days : ", temp);
	char s[10];
	gets_s(s);
	newmonth.numbdays = atoi(s);

	printf("what day is first day? 1.Sat 2.Sun 3.Mon 4.Tue 5.Wed 6.Thu 7.Fri\n");
	char s2[10];
	gets_s(s2);
	int startdayweek = atoi(s2);
	int diff = startdayweek - 2;
	//int monthdays = 1;
	for (int i = 0;;i++) {
		if (i == 0) {
			newmonth.days[i].monthday.day = 0;
			continue;
		}
		newmonth.days[i].monthday.day = i;
		newmonth.days[i].monthday.month = tmonth;
		newmonth.days[i].monthday.year = tyear;
		newmonth.days[i].monthday.weekday = startdayweek;
		if (newmonth.days[i].monthday.weekday == 7) {
			startdayweek -= 7;
			newmonth.days[i].tatil = 1;
			strcpy(newmonth.days[i].reason, "");
			//friday closed
		}
		else
			newmonth.days[i].tatil = 0;
		if (i == newmonth.numbdays) break;
		startdayweek++;
		//monthdays++;



	}

	while (1) {
		//show
		coll;
		printf("\x01b[34m");
		printmonth(newmonth);
		printf("Enter the days clinic is closed (enter one number each time and when your done enter -1) \n");
		char s3[10];
		gets_s(s3);
		int close = atoi(s3);
		if (close == -1) break;
		else if (close <= 0 || close > newmonth.numbdays) {
			printf("Invalid day\n");
			continue;
		}
		newmonth.days[close].tatil = 1;
		printf("reason : \n");
		gets_s(newmonth.days[close].reason);
	}
	co2;
	printf("%s scheduled seccesfully\n", temp);	//save nashode
	//show


	printmonth(newmonth);



	fpp = fopen(themonth, "wb");
	if (!fpp) {
		printf("error");
		exit(0);
	}
	fwrite(&newmonth, sizeof(newmonth), 1, fpp);
	fclose(fpp);
	char cc = getch();
}

void printmonth(callender month) {

	int color = 103;
	printf("\x01b[34m");
	printf("\n");
	printf(" \x01b[%dm", color);
	int x;
	int y = month.days[1].monthday.weekday;

	for (int i = 1;i < y;i++) {
		printf("| -- ");
	}

	for (int i = 1;;i++) {
		printf("|");
		/*if (month.days[i].monthday.day == 0) {
			printf(" -- ");
			continue;
		}*/
		if (month.days[i].tatil == 1) {
			printf("\x01b[31m");
		}
		printf(" %2d ", month.days[i].monthday.day);
		printf("\x01b[34m");


		if (i == month.numbdays) {
			printf("|");
			x = month.days[i].monthday.weekday;
			break;
		}
		if (month.days[i].monthday.weekday == 7)
			printf("|\x01b[106m \n \x01b[%dm", color);

	}
	for (x;x < 7;x++) {
		printf(" -- |");
	}
	printf("\x01b[106m");
	printf("\n");
	printf("\x01b[91m");
	for (int i = 0;;i++) {
		if (month.days[i].monthday.day == 0) continue;

		if (month.days[i].tatil == 1) {
			if (strcmp(month.days[i].reason, "") == 0);
			else
				printf("   %2d. %s\n", month.days[i].monthday.day, month.days[i].reason);
		}

		if (month.days[i].monthday.day == month.numbdays)
			break;


	}
	printf("\x01b[34m");
}

void visitschedual(people list[]) {
	printf("\x01b[35m");
	while (1) {
		struct doctor doc;
		printf("1. All\n2. Choose doctor\n3.Back\n");
		char s[10];
		gets_s(s);
		int a = atoi(s);
		if (a == 3)	return;
		else if (a == 1) {

			showdocvisits(0, doc.viss, 2, 0, list, 0, 0, 0);


		}
		else if (a == 2) {
			printf("Please choose the doctor :\n");
			printf(" \x01b[103m| DoctorID | Doctor name    |\x01b[106m\n");

			for (int i = 1;i < lastID;i++) {
				if (list[i].kind == 1) {
					printf(" \x01b[107m|  %3d     | %s", list[i].ID, list[i].name);
					int k = 15 - strlen(list[i].name);
					for (int kk = 0;kk < k;kk++) {
						printf(" ");
					}
					printf("|\x01b[106m\n");

				}
			}

			char s2[10];
			gets_s(s2);
			int id = atoi(s2);
			if (id >= lastID || id < 1) {
				printf("Invalid input\n");
				continue;
			}
			else if (list[id].kind != 1) {
				printf("Invalid input\n");
				continue;
			}
			char docfile[15];

			itoa(id, docfile, 10);
			strcat(docfile, "doc.bin");
			FILE* fpp = fopen(docfile, "rb");
			if (!fpp) {
				printf("error\n");
				char cc = getch();
				return;
			}

			fread(&doc, sizeof(doc), 1, fpp);
			fclose(fpp);
			showdocvisits(doc.numbviss, doc.viss, 1, 0, list, 0, 0, 0);

		}
		else continue;
		co2;
	}
}

void weektostring(int week, char st[]) {
	switch (week) {
	case 0:
		strcpy(st, "");
		break;
	case 1:
		strcpy(st, "Sat");
		break;
	case 2:
		strcpy(st, "Sun");
		break;
	case 3:
		strcpy(st, "Mon");
		break;
	case 4:
		strcpy(st, "Tue");
		break;
	case 5:
		strcpy(st, "Wed");
		break;
	case 6:
		strcpy(st, "Thu");
		break;
	case 7:
		strcpy(st, "Fri");
		break;
	}
}

void shift(int* numbv, visits v[], int* duration, int id, char name[]) {
	if (*duration == 0) {
		printf("You must determin the duration of each visit first :\n");
		printf("   1)5 min      2)6 min      3)10 min      4)12 min \n");
		printf("   5)15 min     6)20 min     7)30 min      8)60 min \n");
		char s[10];
		gets_s(s);
		int a = atoi(s);
		int dur = 0;
		switch (a) {
		case 1:
			dur = 5;
			break;
		case 2:
			dur = 6;
			break;
		case 3:
			dur = 10;
			break;
		case 4:
			dur = 12;
			break;
		case 5:
			dur = 15;
			break;
		case 6:
			dur = 20;
			break;
		case 7:
			dur = 30;
			break;
		case 8:
			dur = 60;
			break;
		default:
			return;
		}

		*duration = dur;
	}

	int m = today.month;
	char month[25];
	monthtostring(m, month);
	char sall[10];
	itoa(today.year, sall, 10);
	strcat(month, sall);
	strcat(month, ".bin");
	FILE* fp = fopen(month, "rb");
	if (!fp) {
		printf("This month has not been schedualed yet!\nYou cant determine your shifts now\n");
		int cc = getch();
		return;
	}
	callender call;
	fread(&call, sizeof(call), 1, fp);
	fclose(fp);

	while (1) {
		co2;
		printf("month schedule ( %s\b\b\b\b ) :\n", month);
		printmonth(call);
		printf("\n\t1. Set for a day\n\t2. Recurring days in the month\n\t3. Back\n");
		if (*numbv != 0) {
			printf(" \n\tThe days you have determined :\n");
			for (int i = 0;i < *numbv;i++) {
				int w = v[i].day.weekday;
				char ww[5];
				weektostring(w, ww);
				if (v[i].day.month == today.month) {
					printf("\t\t%2d. %s %03d.%02d.%02d from %2d:00 to %2d:00\n"
						, i + 1, ww, v[i].day.year, v[i].day.month, v[i].day.day, v[i].starthour, v[i].endhour);

				}
			}
		}
		printf("\n");
		char s[10];
		gets_s(s);
		int a = atoi(s);

		if (a == 3) return;
		else if (a == 1) {
			printf("Choose a day (between 1 to %d)\n", call.numbdays);
			char s2[10];
			gets_s(s2);
			a = atoi(s2);
			if (a<1 || a>call.numbdays) {
				printf("invalid day");
				int cc = getch();
				continue;
			}
			else if (call.days[a].tatil == 1) {
				printf("Clinic is closed on this day\n");
				int cc = getch();
				continue;
			}
			int flag = 0;

			for (int j = 0;j < *numbv;j++) {						//check if the day hasnt set befor
				if (v[j].day.day == call.days[a].monthday.day) {
					flag = 1;
					break;
				}

			}
			if (flag == 1) {
				printf("This day has been set befor\n");
				int cc = getch();
				continue;
			}
			printf("Choose your work hour on this day\n (this way: start hour > Enter > End hour > Enter)\n");
			char s3[10];
			char s4[10];
			gets_s(s3);
			gets_s(s4);
			int start = atoi(s3);
			int end = atoi(s4);
			int cliniclose = 0;
			if (call.days[a].monthday.weekday == 6) {
				cliniclose = 13;
			}
			else
				cliniclose = 21;
			if (start<8 || end>cliniclose) {
				printf("Clinic is closed in this time\n");
				int cc = getch();
				continue;
			}
			if (start >= end) {
				printf("Invalid times\n");
				int cc = getch();
				continue;
			}
			v[*numbv].starthour = start;
			v[*numbv].endhour = end;
			int temp = 60 / (*duration);
			v[*numbv].numberofvisits = (end - start) * (temp);
			v[*numbv].day = call.days[a].monthday;
			v[*numbv].docID = id;
			strcpy(v[*numbv].docname, name);
			makevisitready(&v[*numbv], *duration);
			*numbv = *numbv + 1;

			printf("done");
			int cc = getch();


		}
		else if (a == 2) {
			printf("Choose a day of week (1.Sat 2.Sun 3.Mon 4.Tue 5.Wed 6.Thu)\n");
			char s2[10];
			gets_s(s2);
			a = atoi(s2);
			if (a < 1 || a>6) {
				printf("invalid weekday");
				int cc = getch();
				continue;
			}

			printf("Choose your work hour on this days\n (this way: start hour > Enter > End hour > Enter)\n");
			char s3[10];
			char s4[10];
			gets_s(s3);
			gets_s(s4);
			int start = atoi(s3);
			int end = atoi(s4);
			int cliniclose = 0;
			if (a == 6) {
				cliniclose = 13;
			}
			else
				cliniclose = 21;
			if (start<8 || end>cliniclose) {
				printf("Clinic is closed in this time\n");
				int cc = getch();
				continue;
			}

			if (start >= end) {
				printf("Invalid times\n");
				int cc = getch();
				continue;
			}

			int temp = 60 / (*duration);
			for (int i = 1;i <= call.numbdays;i++) {
				if (call.days[i].monthday.weekday == a) {
					int flag = 0;
					for (int j = 0;j < *numbv;j++) {						//check if the day hasnt set befor
						if (v[j].day.day == call.days[i].monthday.day) {
							flag = 1;
							break;
						}

					}

					if (call.days[i].tatil == 1) continue;

					if (flag == 1) continue;
					v[*numbv].starthour = start;
					v[*numbv].endhour = end;
					v[*numbv].numberofvisits = (end - start) * (temp);
					v[*numbv].day = call.days[i].monthday;
					v[*numbv].docID = id;
					strcpy(v[*numbv].docname, name);
					makevisitready(&v[*numbv], *duration);
					*numbv = *numbv + 1;
					i += 6;

				}
			}
			printf("done");
			int cc = getch();
		}
		else continue;

		//break;
	}
}

//this function does show reserved visits for admin(mode2) doctor(mode1) patient(mode3) it also cancels (modeplus 1+mode3)		//mode1,mod+2->	prescrpt
void showdocvisits(int numbv, visits v[], int mode, int patid, people list[], int modeplus, int docid, int payment) {
	int cancelnumb = 0;
	int press = 0;
	int gozine = 0;
	//for press	cancel
	int iplus = 0;
	int jplus = 0;
	char canceldocfile[15];

	int color = 35;
	printf("\x01b[%dm", color);


	int m = today.month;
	char month[25];
	monthtostring(m, month);
	char sall[10];
	itoa(today.year, sall, 10);
	strcat(month, sall);
	strcat(month, ".bin");
	FILE* fp = fopen(month, "rb");
	if (!fp) {
		printf("This month has not been schedualed yet!");
		int cc = getch();
		return;
	}
	callender call;
	fread(&call, sizeof(call), 1, fp);
	fclose(fp);
	//int todayweekday = call.days[today.day].monthday.weekday;
	int startweek;
	int endweek;
	for (int i = today.day;;i--) {
		if (i == 1) {
			startweek = 1;
			endweek = 8 - call.days[1].monthday.weekday;
			break;
		}
		if (call.days[i].monthday.weekday == 1) {

			startweek = i;
			endweek = i + 6;
			if (endweek > call.numbdays)
				endweek = call.numbdays;

			break;
		}
	}
tagg:
	cancelnumb = 0;
	press = 0;
	gozine = 0;
	co2;


	while (1) {

		if (modeplus == 2) printf("Writing prescription ... \n");
		else if (modeplus == 1) printf("Canceling ... \n");
		else
			printf("Reserved visits of this week :\n");

		printf("week : %s\b\b\b\b %dth to %dth\n", month, startweek, endweek);
		printf(" \x01b[103m");

		if (mode == 1) {
			printf("   | patientID | appointment time and date     | time of reserve    \n");
			//	    01 |     34    | sun 29.02.401   8:00 to  8:30 | 28.2.401 8:21:13
		}
		if (mode == 2 || mode == 3) {
			printf("   | DocID | Doctor Name       | patientID | appointment time and date     | time of reserve    \n");
			//		01.   45     zahra                  34        sun 29.02.401   8:00 to  8:30   28.2.401 8:21:13
		}

		int k = 1;
		for (int i = 1;i < lastID;i++) {
			char docfile[15];
			struct doctor doc;
			if (mode != 1) {
				if (list[i].kind != 1) continue;

				itoa(list[i].ID, docfile, 10);
				strcat(docfile, "doc.bin");
				FILE * fpp = fopen(docfile, "rb");
				if (!fpp) {
					printf("error\n");
					char cc = getch();
					return;
				}

				fread(&doc, sizeof(doc), 1, fpp);
				fclose(fpp);
				numbv = doc.numbviss;
				v = doc.viss;
			}

			for (int i = 0;i < numbv;i++) {
				//start reading v[i].v									 
				if (v[i].day.day >= startweek && v[i].day.day <= endweek) {
					for (int j = 0;j < v[i].numberofvisits;j++) {
						if (v[i].v[j].patID > 0) {
							//means the time is reserved
							if (mode == 3 && v[i].v[j].patID != patid) continue;
							if (v[i].v[j].prescrpt == 1) continue;
							if (v[i].day.day > today.day && modeplus == 2)	continue;
							if (v[i].day.day < today.day && modeplus == 1)	continue;
							if (v[i].day.month != today.month) continue;

							printf("\x01b[106m \x01b[107m");
							if (v[i].day.day < today.day&& modeplus != 2) printf("\x01b[9m\x01b[47m");

							if (k == gozine) {	 //pres & cancel
								printf("\x01b[46m");
								//save things

								iplus = i;
								jplus = j;

								if (modeplus == 1) {	  //cancel
									strcpy(canceldocfile, docfile);
								}

							}
							printf("%2d |", k);
							if (mode == 2 || mode == 3) {
								printf("  %2d   | %s", v[i].docID, v[i].docname);
								int t = 18 - strlen(v[i].docname);
								for (;t > 0;t--) printf(" ");
								printf("|");
							}
							char w[5];
							weektostring(v[i].day.weekday, w);

							printf("   %3d     | %s %03d.%02d.%02d  %2d:%02d to %2d:%02d | %03d.%02d.%02d %2d:%02d:%02d \n"
								, v[i].v[j].patID, w, v[i].day.year, v[i].day.month, v[i].day.day, v[i].v[j].starthourvis, v[i].v[j].startminvis
								, v[i].v[j].endhourvis, v[i].v[j].endminvis, v[i].v[j].booktime.day.year, v[i].v[j].booktime.day.month
								, v[i].v[j].booktime.day.day, v[i].v[j].booktime.hour, v[i].v[j].booktime.min, v[i].v[j].booktime.sec);
							printf("\x01b[29m");


							k++;
						}
					}
				}


			}
			if (mode == 1) break;
		}
		printf("\x01b[106m");
		if (k == 1) {
			printf("\x01b[31m \x01b[107m     NO visit reserved on this week\x01b[%dm                                 ", color);
			if (mode != 1)
				printf("                            ");
			printf("\n");
		}

		printf("\x01b[106m");
		while (1) {

			char a[2];
			a[1] = '\0';
			a[0] = _getch();
			if (a[0] == -32) a[1] = _getch();
			if (a[0] == '\b') return;

			if (a[0] == '\r') {
				if (modeplus == 1) {	//cancel
					if (gozine == 0) continue;

					FILE * fpp = fopen(canceldocfile, "rb");
					if (!fpp) {
						printf("error\n");
						char cc = getch();
						return;
					}
					struct doctor doctemp;
					fread(&doctemp, sizeof(doctemp), 1, fpp);
					fclose(fpp);

					if (doctemp.viss[iplus].day.day < today.day) {
						printf("This appointment has passed! you cant cancel it\n");
						continue;
					}
					else {
						printf("Are you sure? (y/n) ");
						char s2[10];
						gets_s(s2);
						if (strcmp(s2, "y") != 0)
							continue;

						doctemp.viss[iplus].v[jplus].patID = 0;
						list[patid].wallet += ((doctemp.payment) * 110) / 200;
						updatepeoplefile(list);
						printf("Canceled seccesfuly\n");
						FILE * fpp = fopen(canceldocfile, "wb");
						if (!fpp) {
							printf("error\n");
							char cc = getch();
							return;
						}
						fwrite(&doctemp, sizeof(doctemp), 1, fpp);
						fclose(fpp);
					}



					char cc2 = getch();
					goto tagg;

				}
				else if (modeplus == 2) {	 //pres
					if (gozine == 0) continue;
					addprescrpt(list, docid, v[iplus].v[jplus].patID, payment);
					v[iplus].v[jplus].prescrpt = 1;
					goto tagg;
				}
			}




			//modeplus0
			if (a[1] == 'K') {
				if (startweek == 1) printf("there is no week befor this\n");
				else {
					endweek = startweek - 1;
					startweek = endweek - 6;
					if (startweek < 1) startweek = 1;
					co2;
					gozine = 0;
					break;

				}
			}
			else if (a[1] == 'M') {
				if (endweek == call.numbdays) 	 printf("there is no week after this\n");
				else {
					startweek = endweek + 1;
					endweek = startweek + 6;
					if (endweek > call.numbdays) endweek = call.numbdays;
					co2;
					gozine = 0;
					break;

				}
			}
			else if (a[1] == 'H') {
				if (gozine > 0) {
					gozine--;
					co2;
					break;
				}

			}
			else if (a[1] == 'P') {
				if (gozine < k - 1) {
					gozine++;
					co2;
					break;
				}

			}

		}

	}
}

void makevisitready(visits * visit, int duration) {
	int h = visit->starthour;
	int min = 0;
	int i = 0;
	for (i = 0;i < visit->numberofvisits;i++) {
		visit->v[i].starthourvis = h;
		visit->v[i].startminvis = min;
		visit->v[i].prescrpt = 0;
		min += duration;
		if (min >= 60) {
			min -= 60;
			h++;
		}
		visit->v[i].endhourvis = h;
		visit->v[i].endminvis = min;
		visit->v[i].patID = 0;
	}
	visit->v[i].patID = -1; //means the end of the day
}

void book(people list[], int patid) {
	printf("\x01b[35m");
tg:
	co2;
	printf(" \x01b[103m| DoctorID | Doctor name    |\x01b[106m\n");
	//                     33       zahra
	for (int i = 0;i < lastID;i++) {
		if (list[i].kind == 1) {
			printf(" \x01b[107m|  %3d     | %s", list[i].ID, list[i].name);
			int k = 15 - strlen(list[i].name);
			for (int kk = 0;kk < k;kk++) {
				printf(" ");
			}
			printf("|\x01b[106m\n");

		}
	}
	int a;
	while (1) {
		printf("please choose a dotor (enter the ID)\nEnter -1 to back\n");
		char s[10];
		gets_s(s);
		a = atoi(s);
		if (a == -1) return;
		int valid = 0;
		if (a > 0 && a < lastID) {
			if (list[a].kind == 1) {
				valid = 1;
			}
		}
		if (valid == 0) {
			printf("Invalid ID\n");
			continue;
		}
		break;
	}

	char docfile[15];
	itoa(a, docfile, 10);
	strcat(docfile, "doc.bin");
	struct doctor doc;
	FILE* fp = fopen(docfile, "rb");
	if (!fp) {
		printf("error\n");
		char cc = getch();
		return;
	}
	fread(&doc, sizeof(doc), 1, fp);
	fclose(fp);
	if (doc.numbviss == 0) {
		printf("this doctor hasnt determined shifts in this month\npress any keyto back\n");
		getch();
		co2;
		goto tg;
	}
	else if (today.month != doc.viss[0].day.month) {
		printf("this doctor hasnt determined shifts in this month\npress any keyto back\n");
		getch();
		co2;
		goto tg;
	}
tgg:

	co2;
	printf(" \x01b[103m doctor %d: %s", list[a].ID, list[a].name);
	int k = 15 - strlen(list[a].name);
	for (int i = 0;i < k;i++) printf(" ");
	printf("\x01b[106m\n");
	for (int i = 0;i < doc.numbviss;i++) {
		if (today.day > doc.viss[i].day.day) continue;
		if (today.month == doc.viss[i].day.month) {
			printf(" \x01b[107m%2d.   %2dth %2d:00 to %2d:00 \x01b[106m\n", i + 1, doc.viss[i].day.day, doc.viss[i].starthour, doc.viss[i].endhour);
		}

	}
	printf(" \x01b[107m%2d.   Back                \x01b[106m\n", doc.numbviss + 1);
	int a2;
	while (1) {
		printf("\nChoose one day : ");
		char s[10];
		gets_s(s);
		a2 = atoi(s);
		if (a2 == doc.numbviss + 1) goto tg;
		if (a2 <1 || a2>doc.numbviss) {
			printf("Invalid input\n");
			continue;
		}
		if (doc.viss[a2 - 1].day.day < today.day) {
			printf("INvalid input You cant reserve passed time\n");
			continue;
		}
		break;

	}


	a2--;
tag:
	co2;
	char w[5];
	weektostring(doc.viss[a2].day.weekday, w);
	printf(" \x01b[103m doctor %d : %s    %s  %d.%d.%d", list[a].ID, list[a].name, w,
		doc.viss[a2].day.year, doc.viss[a2].day.month, doc.viss[a2].day.day);
	int k2 = 8 - strlen(list[a].name);
	for (int i = 0;i < k2;i++) printf(" ");
	printf("\x01b[106m\n");

	for (int i = 0;i < doc.viss[a2].numberofvisits;i++) {
		if (doc.viss[a2].v[i].patID == 0)
			printf(" \x01b[107m%2d.    %2d:%02d    to    %2d:%02d         \x01b[106m\n", i + 1, doc.viss[a2].v[i].starthourvis, doc.viss[a2].v[i].startminvis
				, doc.viss[a2].v[i].endhourvis, doc.viss[a2].v[i].endminvis);
	}
	printf(" \x01b[107m%2d.    Back                         \x01b[106m\n\nchoose one visit time: \n", doc.viss[a2].numberofvisits + 1);
	int a3;
	while (1) {
		char s[10];
		gets_s(s);
		a3 = atoi(s);
		a3--;
		if (a3 == doc.viss[a2].numberofvisits)  goto tgg;
		else if (a3<0 || a3>doc.viss[a2].numberofvisits) {
			printf("Invalid input\n");
			continue;
		}
		if (doc.viss[a2].v[a3].patID != 0) {
			printf("This time has been reserved befor \n");
			continue;
		}
		break;
	}
	int money = ((doc.payment) * 110) / 100;
	if (list[patid].wallet < money) {
		printf("You dont have enough money in your wallet please charge your wallet first");
		getch();
		return;
	}
	printf("Are you sure?(y/n) ");
	char s[20];
	gets_s(s);
	if (strcmp(s, "y") == 0) {
		doc.viss[a2].v[a3].patID = patid;
		doc.viss[a2].v[a3].booktime = gettime();
		list[patid].wallet -= money;
		updatepeoplefile(list);
	}
	else
		goto tag;


	fp = fopen(docfile, "wb");
	if (!fp) {
		printf("error\n");
		char cc = getch();
		return;
	}
	fwrite(&doc, sizeof(doc), 1, fp);
	printf("reserved seccessfully\n");

	char cc = getch();


}

void printprescrpt(int patid) {
	co2;
	printf("\x01b[34m");
	char patfile[15];
	itoa(patid, patfile, 10);
	strcat(patfile, "pat.bin");
	FILE* fp = fopen(patfile, "rb");
	if (!fp) {
		printf("error\n");
		getch();
		return;
	}
	struct patient pat;
	fread(&pat, sizeof(pat), 1, fp);
	printf("\x01b[4mprescriptions\x01b[24m\n");
	printf("--------------------------------------------------\n");
	for (int i = 0;i < pat.countprescrpt;i++) {
		printf("%2d . Doctor %2d : %s\nDate and time: %3d.%02d.%02d   %2d:%02d:%02d\n%s\n", i + 1, pat.prescrpt[i].docid
			, pat.prescrpt[i].docname, pat.prescrpt[i].time.day.year, pat.prescrpt[i].time.day.month, pat.prescrpt[i].time.day.day
			, pat.prescrpt[i].time.hour, pat.prescrpt[i].time.min, pat.prescrpt[i].time.sec, pat.prescrpt[i].p);
		printf("--------------------------------------------------\n");
	}
	printf("\npress any key to back");
	getch();
}

void addprescrpt(people list[], int docid, int patid, int payment) {

	printf("write your prescription and when your done enter #\n");
	char patfile[15];
	itoa(patid, patfile, 10);
	strcat(patfile, "pat.bin");
	FILE* fp = fopen(patfile, "rb");
	if (!fp) {
		printf("error\n");
		getch();
		return;
	}
	struct patient pat;
	fread(&pat, sizeof(pat), 1, fp);
	fclose(fp);
	char c;
	int i = 0;
	int k = pat.countprescrpt;
	while (1) {
		pat.prescrpt[k].p[i] = getch();
		if (pat.prescrpt[k].p[i] == '\b') {
			if (i == 0) continue;
			i--;
			printf(" \b\b \b");
			continue;
		}
		if (pat.prescrpt[k].p[i] == '#') {
			pat.prescrpt[k].p[i] = '\0';
			break;
		}
		if (i == 98) {
			printf("*\b");
			continue;
		}
		if (pat.prescrpt[k].p[i] == '\r') {
			pat.prescrpt[k].p[i] = '\n';
		}

		printf("%c", pat.prescrpt[k].p[i]);
		i++;

	}
	printf("\ndone\n");
	pat.prescrpt[k].docid = docid;
	strcpy(pat.prescrpt[k].docname, list[docid].name);
	pat.prescrpt[k].time = gettime();
	pat.countprescrpt++;
	list[docid].wallet += payment;
	updatepeoplefile(list);

	fp = fopen(patfile, "wb");
	if (!fp) {
		printf("error\n");
		getch();
		return;
	}
	fwrite(&pat, sizeof(pat), 1, fp);
	fclose(fp);
}

void updatepeoplefile(people list[]) {
	FILE* fp = fopen("people.bin", "wb");
	if (!fp) {
		printf("error\n");
		getch();
		return;
	}
	fwrite(list, sizeof(struct people), lastID, fp);
	fclose(fp);
}

int rent(int rentmode, int* wallet) {
	char s[10];
	int lmonth = today.month - 1;
	int lyear = today.year;
	if (today.month == 1) {
		lyear--;
		lmonth = 12;
	}
	char lastmonth[25];
	monthtostring(lmonth, lastmonth);
	char y[10];
	itoa(lyear, y, 10);
	strcat(lastmonth, y);
	strcat(lastmonth, ".bin");
	FILE* fp = fopen(lastmonth, "rb");
	if (!fp) {
		printf("there was a problem last month doesnt exist!");
		getch();
		return rentmode;
	}
	callender call;
	fread(&call, sizeof(call), 1, fp);
	fclose(fp);
	int numb = call.numbdays;

	while (1) {

		printf("   1.Extension\n   2.Payment\n");

		gets_s(s);
		int a = atoi(s);



		if (a != 2) {
			if (rentmode == 2 || rentmode == 4) {
				printf("Are you sure?(y/n) you get expelled if you extend this time!\n");
				gets_s(s);
				if (strcmp(s, "y") == 0)
					return 5;
				else
					continue;
			}
			else {
				printf("Are you sure? (y/n) you can extend once in two month\n");
				gets_s(s);
				if (strcmp(s, "y") == 0)
					return 2;
				else
					continue;
			}

		}
		else if (a == 2) {
			int rentpayment = numb * 10;
			if (rentmode == 2 || rentmode == 4) {
				rentpayment *= 2;
			}
			if (rentpayment > * wallet) {
				printf("You dont have enough money\nRent payment is %d$\n", rentpayment);
				continue;
			}
			else {
				*wallet -= rentpayment;
				printf("Payed %d$ successfully\n", rentpayment);
				Sleep(1000);
				return 3;
			}

		}
		//return rentmode;
	}
	printf("there is a problem\n");
	return rentmode;
}

void printbigletters(char* s, int p)
{
	int a = 0;
	int b = 1;
	for (b = 1; b <= 5; b++) {
		a = 0;
		while (*(s + a) != '\0') {
			char c = *(s + a);
			if (c == 'M')
			{

				if (b == 1)
				{
					printf(" ██");
					Sleep(p);
					printf("██     ");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);

				}
				else if (b == 2)
				{
					printf(" ██ ");
					Sleep(p);
					printf("██   ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 3)
				{
					printf(" ██  ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 4)
				{
					printf(" ██   ");
					Sleep(p);
					Sleep(p);
					printf("███   ");
					Sleep(p);
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 5)
				{
					printf("██");
					Sleep(p);
					printf("██       ");
					Sleep(p);
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				a++;
				continue;
			}
			if (c == 'I')
			{
				if (b == 1 || b == 5)
				{
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2 || b == 3 || b == 4)
				{
					printf(" ██  ");
					Sleep(p);
				}

				a++;
				continue;
			}
			if (c == 'N')
			{
				if (b == 1)
				{
					printf(" ██");
					Sleep(p);
					printf("██   ");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2)
				{
					printf(" ██ ");
					Sleep(p);
					printf("██   ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 3)
				{
					printf(" ██  ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 4)
				{
					printf(" ██   ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 5)
				{
					printf("██");
					Sleep(p);
					printf("██   ");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				a++;
				continue;
			}
			if (c == 'E')
			{

				if (b == 1 || b == 5)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2 || b == 4)
				{
					printf("██       ");
					Sleep(p);
					Sleep(p);
					Sleep(p);
				}
				else if (b == 3)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██   ");
					Sleep(p);

				}


				a++;
				continue;
			}
			if (c == 'S')
			{

				if (b == 1 || b == 3 || b == 5)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2)
				{
					printf("██     ");
					Sleep(p);
					Sleep(p);
					Sleep(p);
				}

				else if (b == 4)
				{
					Sleep(p);
					Sleep(p);
					printf("    ██ ");
					Sleep(p);
				}

				a++;
				continue;
			}
			if (c == 'W')
			{

				if (b == 1)
				{
					printf("██");
					Sleep(p);
					printf("██       ");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2)
				{
					printf(" ██   ");
					Sleep(p);
					printf("███   ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 3)
				{
					printf(" ██  ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 4)
				{
					printf(" ██ ");
					Sleep(p);
					printf("██   ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 5)
				{
					printf(" ██");
					Sleep(p);
					printf("██     ");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				a++;
				continue;
			}
			if (c == 'P')
			{

				if (b == 1 || b == 3)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2)
				{
					printf("██  ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}

				else if (b == 4 || b == 5)
				{
					printf("██     ");
					Sleep(p);
					Sleep(p);
					Sleep(p);
				}

				a++;
				continue;
			}
			if (c == 'R')
			{

				if (b == 1 || b == 3)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2 || b == 5)
				{
					printf("██  ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}

				else if (b == 4)
				{
					printf("██ ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}

				a++;
				continue;
			}

			if (c == 'O')
			{
				if (b == 1 || b == 5)
				{
					printf(" ██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 2 || b == 3 || b == 4)
				{
					printf("██    ");
					Sleep(p);
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}


				a++;
				continue;
			}
			if (c == 'L')
			{
				if (b == 1 || b == 2 || b == 3 || b == 4)
				{
					printf("██     ");
					Sleep(p);
					Sleep(p);
					Sleep(p);
				}

				else if (b == 5)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				a++;
				continue;
			}
			if (c == 'T')
			{
				if (b == 1)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2 || b == 3 || b == 4 || b == 5)
				{
					Sleep(p);
					Sleep(p);
					printf("    ██     ");
					Sleep(p);
					Sleep(p);
				}

				a++;
				continue;
			}
			if (c == 'Y')
			{
				if (b == 1)
				{
					printf("███    ");
					Sleep(p);
					printf("███");
					Sleep(p);
				}
				else if (b == 2)
				{
					printf("  ██  ");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 3)
				{
					printf("   ██");
					Sleep(p);
					printf("██   ");
					Sleep(p);
				}
				else if (b == 4 || b == 5)
				{
					Sleep(p);
					printf("    ██    ");
					Sleep(p);
				}

				a++;
				continue;
			}
			if (c == 'U')
			{
				if (b == 1 || b == 2 || b == 3 || b == 4)
				{
					printf("██    ");
					Sleep(p);
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}

				else if (b == 5)
				{
					printf(" ██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				a++;
				continue;
			}
			if (c == 'C') {
				if (b == 1 || b == 5)
				{
					printf(" ██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 2 || b == 4)
				{
					printf("██    ");
					Sleep(p);
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 3) {
					printf("██    ");
					Sleep(p);
					Sleep(p);
					printf("   ");
					Sleep(p);
				}


				a++;
				continue;
			}
			if (c == 'A') {
				if (b == 1 || b == 3)
				{
					printf("██");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}
				else if (b == 2 || b == 5 || b == 4)
				{
					printf("██  ");
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}



				a++;
				continue;
			}
			if (c == 'D') {
				if (b == 1 || b == 5)
				{
					printf("███");
					Sleep(p);
					printf("██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 2 || b == 3 || b == 4)
				{
					printf("██    ");
					Sleep(p);
					Sleep(p);
					printf("██ ");
					Sleep(p);
				}


				a++;
				continue;
			}

			if (c == '1')
			{
				//bomb
				if (b == 1)
				{
					printf("     ██");
					Sleep(p);
					printf("██  ");
					Sleep(p);
				}
				else if (b == 2 || b == 5)
				{
					printf("  ██");
					Sleep(p);
					printf("██     ");
					Sleep(p);
				}
				else if (b == 3 || b == 4)
				{
					printf("██    ");
					Sleep(p);

					printf("██   ");
					Sleep(p);
				}


				a++;
				continue;
			}

			if (c == 'n')
			{
				printf(" ");
			}
			a++;

		}
		printf("\n");
	}


}
