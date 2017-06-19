
#ifndef WIZ_STRING_H
#define WIZ_STRING_H


#include <cstdio>
#include <cstdlib>


#define WIZ_STRING_BUFFER_SIZE 5


namespace wiz {
	class WizString
	{
	private:
		char buffer[WIZ_STRING_BUFFER_SIZE];
		char* str;
		int len;
		bool moved;
	public:
		explicit WizString() {
			buffer[0] = '\0';
			len = 0;
			moved = false;
			str = nullptr;
		}
		explicit WizString(const char* cstr) {

		}
		explicit WizString(const char* cstr, int n) {

		}

		virtual ~WizString()
		{
			if (nullptr != str && false == moved)
			{
				free(str);
			}
		}
	private:
		// copy constructor, assignment -> DON`T
		WizString(const WizString&) = delete;
		void operator=(const WizString&) = delete;
	public:
		bool AssignWithMove(WizString& str) {

		}
		bool AssignNoMove(const WizString& str) {

		}
		bool Assign(const char* cstr) {

		}
	public:
		// +
		// +=
		// ==
		// !=
	public:
		static int BufferSize()
		{
			return WIZ_STRING_BUFFER_SIZE;
		}

		// enterkey is included, return num of \n
		static int GetLine(FILE* file, WizString* str) {

		}
		// enterkey is included, return num of \n
		static int GetMultiLine(FILE* file, WizString* str, int lineNum) {

		}

		template <typename INTEGER>
		static INTEGER ToInteger(const WizString& str)
		{

		}
		template <typename NUMBER>
		static NUMBER ToNumber(const WizString& str)
		{

		}

		template <typename INTEGER>
		static bool ToStringFromInteger(INTEGER val, WizString* str)
		{

		}
		template <typename NUMBER>
		static bool ToStringFromNumber(NUMBER val, WizString* str)
		{

		}

		bool SubString(WizString* result, int begin, int end)
		{

		}

		bool SubStringAndAssign(int begin, int end)
		{

		}

		bool Erase(int index)
		{

		}

		const char* CStr() const
		{

		}

		bool StartsWith(const WizString& start_str) const
		{

		}
		bool StartsWith(const char* start_cstr) const
		{

		}
		bool LastsWith(const WizString& last_str) const
		{

		}
		bool LastsWith(const char* last_cstr) const
		{

		}
		
		void Reverse()
		{

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

		}
		int Front() const
		{

		}

	};
}

#endif

