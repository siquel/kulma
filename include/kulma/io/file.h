#pragma once
#include "../macros.h"
#include "reader_writer.h"
#include <stdint.h>
namespace kulma
{
    KULMA_ERROR_RESULT(KULMA_ERROR_IO_OPEN, KULMA_MAKEFOURCC('I', 'O', 0, 1));
    KULMA_ERROR_RESULT(KULMA_ERROR_IO_READ, KULMA_MAKEFOURCC('I', 'O', 0, 2));
    KULMA_ERROR_RESULT(KULMA_ERROR_IO_WRITE, KULMA_MAKEFOURCC('I', 'O', 0, 3));

    class FileReader : public IFileReader
    {
    public:
        /// \brief Constructor
        FileReader();
        /// \brief Destructor
        ~FileReader() override;

        /// @copydoc IReader::read()
        virtual int32_t read(void* p_data, int32_t p_size, Error* p_err) override;

        /// @copydoc ISeeker::seek()
        virtual int64_t seek(int64_t p_offset = 0, Whence::Enum p_whence = Whence::Current) override;

        /// @copydoc IReaderOpener::open()
        virtual bool open(const char* p_filePath, Error* p_err) override;

        /// @copydoc IClose::close()
        virtual void close() override;
    private:
#if KULMA_PLATFORM_WINDOWS
        void* m_file; // HANDLE
#elif KULMA_PLATFORM_LINUX
        FILE* m_file;
#endif
    };

    class FileWriter : public IFileWriter
    {
    public:
        /// \brief Constructor
        FileWriter();

        /// \brief Destructor
        ~FileWriter() override;

        /// @copyedoc IWriter::write()
        virtual int32_t write(const void* p_data, int32_t p_size, Error* p_err) override;

        /// @copydoc ISeeker::seek()
        virtual int64_t seek(int64_t p_offset = 0, Whence::Enum p_whence = Whence::Current) override;

        /// @copydoc IWriterOpener::open()
        virtual bool open(const char* p_filePath, bool p_append, Error* p_err) override;

        /// @copydoc IClose::close()
        virtual void close() override;
    private:
#if KULMA_PLATFORM_WINDOWS
        void* m_file; // HANDLE
#elif KULMA_PLATFORM_LINUX
        FILE* m_file;
#endif
    };
}

