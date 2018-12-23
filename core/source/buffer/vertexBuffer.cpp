#include <stdafx.h>
#include <ogele.h>
namespace ogele {
	BufferBase::BufferBase(BufferDataType dataType, size_t structSize, int size) {
		switch (dataType) {
		case BufferDataType::Byte:
			m_elemSize = sizeof(signed char);
			break;
		case BufferDataType::Short:
			m_elemSize = sizeof(signed short);
			break;
		case BufferDataType::Int:
			m_elemSize = sizeof(signed int);
			break;
		case BufferDataType::UnsignedByte:
			m_elemSize = sizeof(unsigned char);
			break;
		case BufferDataType::UnsignedShort:
			m_elemSize = sizeof(unsigned short);
			break;
		case BufferDataType::UnsignedInt:
			m_elemSize = sizeof(unsigned int);
			break;
		case BufferDataType::Float:
			m_elemSize = sizeof(float);
			break;
		case BufferDataType::Double:
			m_elemSize = sizeof(double);
			break;
		default:
			throw std::runtime_error("Unsupported vertex buffer data type");
		}
		m_size = size;
		m_dataType = dataType;
		m_structSize = structSize;
		m_handle = 0;
		glGenBuffers(1, &m_handle);
		GLErr();
	}

	BufferBase::~BufferBase() {
		glDeleteBuffers(1, &m_handle);
		GLErr();
	}

}