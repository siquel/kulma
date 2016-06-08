#pragma once

#include "../macros.h"
#include "../error.h"
#include <stdint.h>

namespace kulma
{
    struct Whence
    {
        enum Enum
        {
            Begin,
            Current,
            End
        };
    };
    /// \brief Interface for closing 
    struct KULMA_NO_VTABLE ICloser
    {
        /// \brief Destructor
        virtual ~ICloser() = 0;
        /// \brief Closes the stream
        virtual void close() = 0;
    };

    inline ICloser::~ICloser() {}

    /// \brief Interface for seeking in stream
    struct KULMA_NO_VTABLE ISeeker
    {
        /// \brief Destructor
        virtual ~ISeeker() = 0;
        /// \brief Move the file pointer to the given position from given whence
        /// 
        /// \param p_offset The cursor position to set
        /// \param p_whence Origin
        ///
        /// \return The new position
        virtual int64_t seek(int64_t p_offset = 0, Whence::Enum p_whence = Whence::Current) = 0;
    };

    inline ISeeker::~ISeeker() {}

    /// \brief Interface for reading data
    struct KULMA_NO_VTABLE IReader
    {
        /// \brief Destructor
        virtual ~IReader() = 0;
        /// \brief Reads data
        /// 
        /// \param p_data Pointer to a pre-allocated data buffer
        /// \param p_size Size of data to read in bytes
        /// \param p_err Error handler
        ///
        /// \return Amount of data read in bytes
        virtual int32_t read(void* p_data, int32_t p_size, Error* p_err) = 0;
    };

    inline IReader::~IReader() {}

    /// \brief Interface for opening file for reading
    struct KULMA_NO_VTABLE IReaderOpener
    {
        /// \brief Destructor
        virtual ~IReaderOpener() = 0;
        /// \brief Opens file for reading
        /// 
        /// \param p_filePath File path
        /// \param p_err Error handler
        ///
        /// \return True if opened successfully
        virtual bool open(const char* p_filePath, Error* p_err) = 0;
    };

    inline IReaderOpener::~IReaderOpener() {}

    /// \brief Interface for writing
    struct KULMA_NO_VTABLE IWriter
    {
        /// \brief Destructor
        virtual ~IWriter() = 0;
        /// \brief Write data
        ///
        /// \param p_data Data to write
        /// \param p_size Size of data in bytes
        /// \param p_err Error handler
        /// 
        /// \return Amount of bytes written
        virtual int32_t write(const void* p_data, int32_t p_size, Error* p_err) = 0;
    };

    inline IWriter::~IWriter() {}

    /// \brief Interface for opening file for writing
    struct KULMA_NO_VTABLE IWriterOpener
    {
        /// \brief Destructor
        virtual ~IWriterOpener() = 0;
        /// \brief Opens file for writing
        /// 
        /// \param p_filePath File path
        /// \param p_err Error handler
        /// \param p_append Append to file or should the file be truncated
        /// 
        /// \return True if opened successfully
        virtual bool open(const char* p_filePath, bool p_append, Error* p_err) = 0;
    };

    inline IWriterOpener::~IWriterOpener() {}

    /// \brief Interface for file reading
    struct KULMA_NO_VTABLE IFileReader : public IReader, public ISeeker,  public IReaderOpener, public ICloser
    {

    };

    /// \brief Interface for file writing
    struct KULMA_NO_VTABLE IFileWriter : public IWriter, public ISeeker, public IWriterOpener, public ICloser
    {

    };
}