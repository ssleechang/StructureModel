#pragma once

#include <string>
#include <assert.h>
#include <atlstr.h>

///Description		:	EFS_Buffer
///Madeby			:	STRIKER  (박광호)
///Mail				:	forinnovate@gmail.com
///Date				:	2009/10/02
namespace EFS	{

	class EFS_Buffer
	{
	public:
		typedef unsigned short	MessageSizeType;

		enum eMessageOffset
		{
			MessageActualSizePosition = 0,
			DataPosition = MessageActualSizePosition + sizeof(MessageSizeType),
		};

	private:
		unsigned int				_Offset;
		unsigned int				_ExtendBufferSize;
		char						*_pBuffer;
		unsigned int				_Size;

	protected:
		//저장되어야 하는것은 현재 Actual DataSize?//


		unsigned int				_Capacity;

	public:
		EFS_Buffer()
		{
			_Capacity = 0;
			_ExtendBufferSize = 128;
			_Size = 0;
			Init();


		}

		EFS_Buffer(char* Data, int Length)
		{
			SetBuffer(Data, Length);
		}

		EFS_Buffer(int Capacity)
		{
			//EFS_Alloc(_pBuffer, char[Length]);
			Create(Capacity);
		}

		~EFS_Buffer()
		{
			Release();
		}  

		bool Init()
		{
			_Offset = 0;
			//_Capacity = 0;
			//_Size = 0;
			//Set Size's Buffer's Position.
			//_ActualSize = (MessageSizeType*)GetBufferAt((unsigned int)MessageActualSizePosition);
			return true;
		}

		void ExtendBuffer()
		{
			int bufferSize = _Capacity + _ExtendBufferSize;
			char* extendBuffer = new char[bufferSize];
			memcpy(extendBuffer,_pBuffer, _Size);
			delete _pBuffer;
			_pBuffer = extendBuffer;
			_Capacity = bufferSize;
		}


	public:
		void Create(unsigned int Capacity)
		{
			//_pBuffer = EFS_Allocate(_pBuffer, Capacity);
			_pBuffer = new char[Capacity];
			//EFS_Alloc(_pBuffer, char[Capacity]);
			_Capacity = Capacity;
		}

		void SetBuffer(char* Element, unsigned int Length)
		{
			_pBuffer = Element;
			Init();
		}

		int CopyBuffer(const char* Buffer, unsigned int Length)
		{
			return this->WriteBuffer((void*)Buffer, 0, Length);
		}


		void Release()
		{
			_pBuffer = 0;
		}

		char*						GetBuffer() const
		{		
			return _pBuffer;		
		}

		char*						GetTrimedBuffer()
		{
			return _pBuffer;
		}

		//현재 Total Capacity
		const MessageSizeType		GetCapacity()
		{		
			return _Capacity;		
		}

		int							GetSize()
		{
			return _Size;
		}

	public:
		int							WriteBuffer(void *pBuf, unsigned int Offset, unsigned int Length);
		int							ReadBuffer(void *pBuf, unsigned int Offset, unsigned int Length);

		template <typename T>
		int  Read(T& Data)
		{
			int TSize = sizeof(T);

			int result = ReadBuffer((void*)&Data, _Offset, TSize);

			if ( 0 == result )
			{
				_Offset += TSize;
			}

			return result;
		}

		template <class T>
		int  Read(T* Data)
		{
			int TSize = sizeof(Data);

			int result = ReadBuffer(Data, _Offset, TSize);

			if ( 0 == result )
			{
				_Offset += TSize;
			}

			return result;
		}


		template <typename T>
		int Write(T& Data)
		{
			int TSize = sizeof(T);

			int result =  WriteBuffer((void*)&Data, _Offset, TSize);

			if ( 0 ==  result )
				_Offset += TSize;

			return result;

		}

		template <class T>
		int Write(T* Data)
		{
			int TSize = sizeof(Data);

			int result =  WriteBuffer(Data, _Offset, TSize);

			if ( 0 ==  result )
				_Offset += TSize;
			return 0;
		}

		template <class T>
		EFS_Buffer& operator << (T *Msg)
		{
			Write(Msg);
			return *this;
		}

		template <typename T>
		EFS_Buffer& operator << (T &Msg)
		{
			Write(Msg);
			return *this;
		}

		//wchar_T용
		template <>
		EFS_Buffer& operator << (CString &Msg)
		{
			unsigned short sLength = Msg.GetLength();
			WriteBuffer( &sLength, _Offset, sizeof(sLength));
			_Offset += sizeof(sLength);

			int writeSize = sizeof(wchar_t)*sLength;
			LPTSTR MsgBuffer = Msg.GetBufferSetLength(sLength);
			WriteBuffer(MsgBuffer, _Offset ,writeSize);
			_Offset += writeSize;
			return *this;
		}


		template <class T>
		EFS_Buffer& operator >> (T *Msg)
		{
			Read(Msg);
			return *this;
		}

		template <typename T>
		EFS_Buffer& operator >> (T &Msg)
		{
			Read(Msg);
			return *this;
		}

		//wchar_T용
		template <>
		EFS_Buffer& operator >> ( CString &Msg)
		{
			unsigned short sLength;
			ReadBuffer(&sLength, _Offset, sizeof(sLength));
			_Offset += sizeof(sLength);

			int readSize = sizeof(wchar_t)*sLength;
			LPTSTR MsgBuffer = Msg.GetBufferSetLength(sLength);
			ReadBuffer(MsgBuffer,_Offset, readSize);
			//Msg.GetBufferSetLength(sLength);
			_Offset += readSize;
			return *this;
		}

	protected:
		int							SetBufferAt(char* pBuffer, unsigned int Target, unsigned int Length);
		char*						GetBufferAt(unsigned int Target);
	};


	inline int		EFS_Buffer::WriteBuffer(void *pBuf, unsigned int StartPosition, unsigned int Length)
	{
		//Overflow check//
		unsigned int WriteEndPoint = StartPosition + Length;
		while ( _Capacity < WriteEndPoint )
		{
			ExtendBuffer();
		}

		char* pWriteBuffer = GetBufferAt(StartPosition);
		memcpy(pWriteBuffer, pBuf, Length);

		if ( _Size < WriteEndPoint)
			_Size = WriteEndPoint;

		return 0;
	}

	inline int		EFS_Buffer::ReadBuffer(void *pBuf, unsigned int Offset,  unsigned int Length)
	{
		if ( _Size < Offset + Length)
		{
			return -1;
		}

		char* pReadBuffer = GetBufferAt(Offset);
		memcpy(pBuf, pReadBuffer, Length);

		return 0;
	}

	inline	int	EFS_Buffer::SetBufferAt(char* pBuffer, unsigned int StartPosition, unsigned int Length)
	{
		return WriteBuffer(pBuffer, StartPosition, Length);
	}


	inline	char*	EFS_Buffer::GetBufferAt(unsigned int Target)
	{
		return _pBuffer + Target;
	}


};//Namespace