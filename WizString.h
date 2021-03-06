
#ifndef WIZ_STRING_H
#define WIZ_STRING_H

#define _CRT_SECURE_NO_WARNINGS

#include "StringBuilder.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
//#include <string>


#define WIZ_STRING_BUFFER_SIZE 5


namespace wiz {
	class WizString
	{
	private:
		char buffer[WIZ_STRING_BUFFER_SIZE];
		char* str;
		int len;
		int moved;
	private:
		void init(const char* cstr, int n)
		{
			moved = 0;

			if (0 == n) {
				len = 0;
				str = "";
				buffer[0] = '\0';
			}
			else {
				if (n > WIZ_STRING_BUFFER_SIZE) {
					str = (char*)malloc(sizeof(char)*(n + 1));
					strncpy(str, cstr, n + 1);
					len = n;
				}
				else {
					str = "";
					strncpy(buffer, cstr, n + 1);
					len = n;
				}
			}
		}
		void remove()
		{
			if (0 != len) {
				if (len > WIZ_STRING_BUFFER_SIZE && 0 == moved) {
					free(str);

					str = "";
					buffer[0] = '\0';
					len = 0;
					moved = -1;
				}
				else if (0 == moved) {
					buffer[0] = '\0';
					str = "";
					len = 0;
					moved = -2;
				}
			}
		}
	public:
		explicit WizString() {
			init("", 0);
		}
		explicit WizString(const char* cstr) {
			init(cstr, strlen(cstr));
		}
		WizString(const WizString& other)
		{
			init("", 0);
			AssignNoMove(other);
		}
		WizString(WizString&& other)
		{
			len = other.len;
			other.len = 0;
			moved = other.moved;
			other.moved = true;
			str = other.str;
			other.str = "";

			for (int i = 0; i < WIZ_STRING_BUFFER_SIZE; ++i) {
				buffer[i] = other.buffer[i];
			}
		}

		virtual ~WizString()
		{
			remove();
		}
	private:
		// assignment -> DON`T
		void operator=(const WizString&) = delete;
		// move assignment -> DON`T
		void operator=(WizString&&) = delete;
	public:
		bool AssignWithMove(WizString& str) {
			if (str.moved)
			{
				return false;
			}

			if (this->len != str.len || str.moved) {
				this->remove();
				this->len = str.len;
				if (str.len > WIZ_STRING_BUFFER_SIZE) {
					this->str = (char*)malloc(sizeof(char)*(str.len + 1));
				}
			}
			this->moved = str.moved;
			str.moved = true;
			strncpy(this->CStr(), str.CStr(), str.len + 1);
			
			return true;
		}
		bool AssignNoMove(const WizString& str) {
			if (false == str.moved)
			{
				return false;
			}

			if (this->len != str.len || str.moved) {
				this->remove();
				this->len = str.len;
				if (str.len > WIZ_STRING_BUFFER_SIZE) {
					this->str = (char*)malloc(sizeof(char)*(str.len + 1));
				}
			}
			this->moved = str.moved;
			strncpy(this->CStr(), str.CStr(), str.len + 1);
			
			return true;
		}
		bool Assign(const char* cstr) {
			const int cstr_len = strlen(cstr);

			if (this->len != cstr_len ) {
				this->remove();
				this->len = cstr_len;
				if (cstr_len > WIZ_STRING_BUFFER_SIZE) {
					this->str = (char*)malloc(sizeof(char)*(cstr_len + 1));
				}
			}
			this->moved = false; //
			strncpy(this->CStr(), cstr, cstr_len + 1);

			return true;
		}
	public:
		// +
		// + with const char*
		// + with const char*, friend
		friend WizString operator+(const WizString& str1, const WizString& str2)
		{
			WizString temp;

			temp.len = str1.len + str2.len;
			temp.moved = 0;

			if (temp.len == 0) {
				temp.buffer[0] = '\0';
				temp.str = "";
			}
			else {
				if (temp.len > WIZ_STRING_BUFFER_SIZE) {
					temp.str = (char*)malloc(sizeof(char)*(temp.len + 1));
					strncpy(temp.str, str1.CStr(), str1.len + 1);
					strncpy(temp.str + str1.len, str2.CStr(), str2.len + 1);
				}
				else {
					strncpy(temp.buffer, str1.CStr(), str1.len + 1);
					strncpy(temp.buffer + str1.len, str2.CStr(), str2.len + 1);
				}
			}

			return temp;
		}
		friend WizString operator+(const WizString& str1, const char* cstr)
		{
			const int cstr_len = strlen(cstr);
			WizString temp;

			temp.len = str1.len + cstr_len;
			temp.moved = 0;

			if (temp.len == 0) {
				temp.buffer[0] = '\0';
				temp.str = "";
			}
			else {
				if (temp.len > WIZ_STRING_BUFFER_SIZE) {
					temp.str = (char*)malloc(sizeof(char)*(temp.len + 1));
					strncpy(temp.str, str1.CStr(), str1.len + 1);
					strncpy(temp.str + str1.len, cstr, cstr_len + 1);
				}
				else {
					strncpy(temp.buffer, str1.CStr(), str1.len + 1);
					strncpy(temp.buffer + str1.len, cstr, cstr_len + 1);
				}
			}
			return temp;
		}
		friend WizString operator+(const char* cstr, const WizString& str)
		{
			WizString temp;
			temp.AssignNoMove(str + cstr);
			return temp;
		}
		
		// +=
		void operator+=(const WizString& str)
		{
			const int len = this->len + str.len;
			WizString temp;
			temp.moved = false;
			
			temp.len = this->len + str.len;
			if (temp.len > WIZ_STRING_BUFFER_SIZE) {
				temp.str = (char*)malloc(sizeof(char)*(temp.len + 1));
			}
			strncpy(temp.CStr(), this->CStr(), this->len + 1);
			strncpy(temp.CStr() + this->len, str.CStr(), str.len + 1);

			remove();

			this->AssignWithMove(temp);
		}
		// += with const char*
		void operator+=(const char* cstr)
		{
			const int cstr_len = strlen(cstr);

			const int len = this->len + cstr_len;
			WizString temp;
			temp.moved = false;

			temp.len = this->len + cstr_len;
			if (temp.len > WIZ_STRING_BUFFER_SIZE) {
				temp.str = (char*)malloc(sizeof(char)*(temp.len + 1));
			}
			strncpy(temp.CStr(), this->CStr(), this->len + 1);
			strncpy(temp.CStr() + this->len, cstr, cstr_len + 1);

			remove();

			this->AssignWithMove(temp);
		}
		// ==
		bool operator==(const WizString& str)
		{
			if (this->len != str.len) { return 0; }
			return 0 == strcmp(this->CStr(), str.CStr());
		}
		// == with const char*
		bool operator==(const char* str)
		{
			if (this->len != strlen(str)) { return 0; }
			return 0 == strcmp(this->CStr(), str);
		}
		// == with const char*, friend
		friend bool operator==(const char* str, const WizString& wizStr)
		{
			if (wizStr.len != strlen(str)) { return 0; }
			return 0 == strcmp(wizStr.CStr(), str);
		}
		// !=
		bool operator!=(const WizString& str)
		{
			if (this->len == str.len) { return 0; }
			return 0 != strcmp(this->CStr(), str.CStr());
		}
		// != with const char*
		bool operator!=(const char* str)
		{
			if (this->len == strlen(str)) { return 0; }
			return 0 != strcmp(this->CStr(), str);
		}
		// != with const char*, friend
		friend bool operator!=(const char* str, const WizString& wizStr)
		{
			if (wizStr.len == strlen(str)) { return 0; }
			return 0 != strcmp(wizStr.CStr(), str);
		}
	private:
		static int find(const char* cstr, char x, int before, int n)
		{
			int i = 0;

			for (i = before + 1; i < n; ++i) {
				if (cstr[i] == x) {
					return i;
				}
			}

			return -1;
		}
	public:
		static int BufferSize()
		{
			return WIZ_STRING_BUFFER_SIZE;
		}

		// enterkey is included, return valid
		static void GetLine(FILE* file, WizString* strVec) {
			GetMultiLine(file, strVec, 1);
		}
		// enterkey is included, return valid
		static void GetMultiLine(FILE* file, WizString* strVec, const int lineNum) {
			int real_line_num = -1;
			const int line_max = lineNum;
			const int num = 102400;
			StringBuilder builder(num * 10, "", 0);
			int real_count;
			int i;
			int size = 0;
			const char* builder_cstr = NULL;
			WizString temp;
			int temp2;
			const char* temp3 = NULL;
			int chk = 1;
			int offset = 0; // for backward
			int temp5;
			int fpos = ftell(file);

			i = 0;
			temp2 = -1;
			temp5 = -1;

			while (true) {
				if (chk) {
					StringBuilder temp_builder(num, "", 0);
					real_count = fread((void*)temp_builder.Str(), sizeof(char), num, file);

					temp_builder.SetLength(real_count);
					temp_builder.SetBuffer(real_count, '\0');

					builder.Append(temp_builder.Str(), temp_builder.Size());
				}

				builder_cstr = builder.Str();
				size = builder.Size();

				for (; i < line_max; ++i) {
					int chk_find = WizString::find(builder_cstr, '\n', temp2, size);
					
					temp5 = chk_find;

					if (chk_find != -1) {
						temp2 = chk_find;
					}
					else {
						chk = 1;
						break;
					}
				}

				// ... enterkey ... 
				// ... find_idx ... 
				if (line_max == i || real_count != num) {
					real_line_num = i;

					if (temp5 == -1) {
						temp2 = builder.Size() - 1;
					}

					builder.Divide(temp2 + 1);

					temp3 = builder.Str();
					temp.init(temp3, strlen(temp3));
	
					builder.LeftShift(temp2 + 1);
				}
				else {
					continue;
				}

				if (line_max == i) {
					fpos = fpos + temp2 + 1 + i;
					fseek(file, fpos, SEEK_SET);
				}
				else {
					//
				}

				strVec->AssignWithMove(temp);

				break;
			}
		}

		static long long ToInteger(const WizString& str)
		{
			return atoll(str.CStr());
		}
		static long double ToNumber(const WizString& str)
		{
			return strtold(str.CStr(), nullptr);
		}

		static bool ToString(long long x, WizString* str)
		{
			StringBuilder temp(1024, "", 0);
			int size = 1024;

			while (1) {
				int chk = snprintf(temp.Str(), size, "%lld", x);
				if (!(0 <= chk && chk < size)) {
					size = size * 2;
					temp = StringBuilder(size, "", 0);
				}
				else {
					temp.SetLength(chk);
					break;
				}
			}

			str->Assign(temp.Str());
			return true;
		}
		static bool ToString(long double x, WizString* str)
		{
			StringBuilder temp(1024, "", 0);
			int size = 1024;
			
			while (true) {
				int chk = snprintf(temp.Str(), size, "%Lf", x);
				if (!(0 <= chk && chk < size)) {
					size = size * 2;
					temp = StringBuilder(size, "", 0);
				}
				else {
					temp.SetLength(chk);
					break;
				}
			}

			str->Assign(temp.Str());

			return true;
		}

		bool SubString(WizString* result, int begin, int end)
		{
			WizString temp;

			temp.len = (end - 1) - begin + 1;
			if (temp.len <= 0) { return false; }
			temp.moved = 0;

			if (temp.len > WIZ_STRING_BUFFER_SIZE) {
				temp.str = (char*)malloc(sizeof(char) * (temp.len + 1));

				strncpy(temp.str, CStr() + begin, end - begin);
				temp.str[end - begin] = '\0';
			}
			else {
				strncpy(temp.buffer, CStr() + begin, end - begin);
				temp.buffer[end - begin] = '\0';
			}
			result->AssignWithMove(temp);
			
			return true;
		}

		bool SubStringAndAssign(int begin, int end)
		{
			WizString temp;
			temp.len = (end - 1) - begin + 1;
			temp.moved = 0;

			if (temp.len > WIZ_STRING_BUFFER_SIZE) {
				temp.str = (char*)malloc(sizeof(char) * (temp.len + 1));
			}
			strncpy(temp.CStr(), this->CStr() + begin, end - begin + 1);

			this->remove();

			this->AssignNoMove(temp);

			return true;
		}

		bool Erase(int idx)
		{
			for (int i = idx; i < this->len - 1; ++i) {
				this->CStr()[i] = this->CStr()[i + 1];
			}
			this->CStr()[this->len - 1] = '\0';
			this->len--;

			if (this->len == WIZ_STRING_BUFFER_SIZE) {
				strncpy(this->buffer, this->str, WIZ_STRING_BUFFER_SIZE + 1);

				free(this->str);

				this->str = "";
				this->moved = -3;
			}
		}

		const char* CStr() const
		{
			if (len > WIZ_STRING_BUFFER_SIZE) { return str; }
			return buffer;
		}
		char* CStr()
		{
			if (len > WIZ_STRING_BUFFER_SIZE) { return str; }
			return buffer;
		}

		bool StartsWith(const WizString& start_str) const
		{
			int result = true;

			if (this->len < start_str.len) {
				return false;
			}

			for (int i = 0; i < start_str.len; ++i) {
				if (this->CStr()[i] == start_str.CStr()[i]) {
					//
				}
				else {
					result = false;
					break;
				}
			}

			return result;
		}
		bool StartsWith(const char* start_cstr) const
		{
			bool result = true;
			const int start_cstr_len = strlen(start_cstr);

			if (this->len < start_cstr_len) {
				return false;
			}

			for (int i = 0; i < start_cstr_len; ++i) {
				if (this->CStr()[i] == start_cstr[i]) {
					//
				}
				else {
					result = false;
					break;
				}
			}

			return result;
		}
		bool LastsWith(const WizString& last_str) const
		{
			bool result = true;
			int j;

			if (this->len < last_str.len) {
				return 0;
			}

			j = last_str.len - 1;
			for (int i = this->len - 1; i >= 0 && j >= 0; --i) {
				if (this->CStr()[i] == last_str.CStr()[j]) {
					--j;
				}
				else {
					result = false;
					break;
				}
			}

			return result;
		}
		bool LastsWith(const char* last_cstr) const
		{
			bool result = true;
			int j;
			const int last_cstr_len = strlen(last_cstr);

			if (this->len < last_cstr_len) {
				return 0;
			}

			j = last_cstr_len - 1;
			for (int i = this->len - 1; i >= 0 && j >= 0; --i) {
				if (this->CStr()[i] == last_cstr[j]) {
					--j;
				}
				else {
					result = false;
					break;
				}
			}

			return result;
		}
		
		void Reverse()
		{
			char ch;

			for (int i = 0; i < this->len / 2; ++i) {
				ch = this->CStr()[i];
				this->CStr()[i] = this->CStr()[this->len - 1 - i];
				this->CStr()[this->len - 1 - i] = ch;
			}
		}

		int Size()const
		{
			return len;
		}
		bool Empty()const
		{
			return 0 == len;
		}
		int Back() const
		{
			if (len > WIZ_STRING_BUFFER_SIZE) { return str[len - 1]; }
			return buffer[len - 1];
		}
		int Front() const
		{
			if (len > WIZ_STRING_BUFFER_SIZE) { return str[0]; }
			return buffer[0];
		}
	private:
		bool comp(const char* x, const char* y, int n)
		{
			for (int i = 0; i < n; ++i) {
				if (x[i] != y[i]) {
					return false;
				}
			}
			return true;
		}
	public:
		bool Replace(const WizString& mark, const WizString& changed, WizString* result, StringBuilder* builder)
		{
			const char* pStr = this->CStr();

			builder->Clear();

			// chk??
			if (mark.Empty()) {
				return false;
			}

			for (int i = 0; i < this->Size(); i++) {
				if (strlen(pStr + i) >= mark.Size()
					&& comp(pStr + i, mark.CStr(), mark.Size()))
				{
					builder->Append(changed.CStr(), changed.Size());
					i = i + mark.Size() - 1;
				}
				else
				{
					builder->AppendChar(this->CStr()[i]);
				}
			}

			result->Assign(builder->Str());

			return true;
		}
		void AddSpace(WizString* result, StringBuilder* builder)
		{
			builder->Clear();

			for (int i = 0; i < this->Size(); ++i)
			{
				if ('=' == this->CStr()[i]) {
					builder->Append(" = ", 3);
				}
				else if ('{' == this->CStr()[i]) {
					builder->Append(" { ", 3);
				}
				else if ('}' == this->CStr()[i]) {
					builder->Append(" } ", 3);
				}
				else {
					builder->AppendChar(this->CStr()[i]);
				}
			}

			result->Assign(builder->Str());
		}
	};
}

#endif

