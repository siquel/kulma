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
            Read,
            Write
        };
    };

    struct KULMA_NO_VTABLE IFile 
    {
        /// \brief Destructor
        virtual ~IFile() = 0;

        /// \brief Opens the file for reading/writing
        /// 
        /// \param p_path The file path
        /// \param p_mode In which mode the file should be opened
        ///
        /// \return True if opened successfully
        virtual bool open(const char* p_path, FileOpenMode::Enum p_mode) = 0;

        virtual int64_t seek(int64_t p_offset, Whence::Enum p_whence) = 0;

        /// \brief Closes the file
        virtual void close() = 0;

        /// \brief Checks if the file is open
        ///
        /// \return True if the file is open
        virtual bool is_open() const = 0;

        /// \brief
        virtual int32_t read(void* p_data, int32_t p_size) = 0;
    };

    KULMA_ERROR_RESULT(KULMA_ERROR_IO_OPEN, KULMA_MAKEFOURCC('I', 'O', 0, 1));
    KULMA_ERROR_RESULT(KULMA_ERROR_IO_READ, KULMA_MAKEFOURCC('I', 'O', 0, 2));
    KULMA_ERROR_RESULT(KULMA_ERROR_IO_WRITE, KULMA_MAKEFOURCC('I', 'O', 0, 3));

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
    private:
        IFile* m_file;
    };
}

