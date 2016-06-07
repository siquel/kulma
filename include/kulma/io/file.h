#pragma once
#include "../macros.h"
#include "reader_writer.h"
#include <stdint.h>
namespace kulma
{
    struct FileOpenMode
    {
        enum Enum
        {
            Open,
            Write,
            Truncate
        };
    };

    struct KULMA_NO_VTABLE IFile 
    {
        virtual ~IFile() = 0;

        virtual void close() = 0;

        /// \brief 
        virtual int32_t read(void* p_data, int32_t p_size) = 0;
    };

    class FileReader : public IFileReader
    {
    public:
        FileReader();
        /// \brief Destructor
        ~FileReader() override;

        /// @copydoc IReader::read()
        virtual int32_t read(void* p_data, int32_t p_size, Error* p_err) override;

        /// @copydoc ISeeker::seek()
        virtual int64_t seek(int64_t p_offset = 0, Whence::Enum p_whence = Whence::Current) override;

        /// @copydoc IReaderOpen::open()
        virtual bool open(const char* p_filePath, Error* p_err) override;

        /// @copydoc IClose::close()
        virtual void close() override;
    };
}

