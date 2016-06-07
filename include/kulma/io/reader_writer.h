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
    struct KULMA_NO_VTABLE Closer
    {
        /// \brief Destructor
        virtual ~Closer() = 0;
        /// \brief Closes the stream
        virtual void close() = 0;
    };

    inline Closer::~Closer() {}

    /// \brief Interface for seeking in stream
    struct KULMA_NO_VTABLE Seeker
    {
        /// \brief Destructor
        virtual ~Seeker() = 0;
        /// \brief Move the file pointer to the given position from given whence
        /// 
        /// \param p_offset The cursor position to set
        /// \param p_whence Origin
        ///
        /// \return The new position
        virtual int64_t seek(int64_t p_offset = 0, Whence::Enum p_whence = Whence::Current) = 0;
    };

    inline Seeker::~Seeker() {}

    /// \brief Interface for reading data
    struct KULMA_NO_VTABLE Reader
    {
        /// \brief Destructor
        virtual ~Reader() = 0;
        /// \brief Reads data
        /// 
        /// \param p_data Pointer to a pre-allocated data buffer
        /// \param p_size Size of data to read in bytes
        /// \param p_err Error handler
        ///
        /// \return Amount of data read in bytes
        virtual int32_t read(void* p_data, int32_t p_size, Error* p_err) = 0;
    };

    inline Reader::~Reader() {}

    /// \brief Interface for opening file for reading
    struct KULMA_NO_VTABLE ReaderOpener
    {
        /// \brief Destructor
        virtual ~ReaderOpener() = 0;
        /// \brief Opens file for reading
        /// 
        /// \param p_filePath File path
        /// \param p_err Error handler
        ///
        /// \return True if opened successfully
        virtual bool open(const char* p_filePath, Error* p_err) = 0;
    };

    inline ReaderOpener::~ReaderOpener() {}

    /// \brief Interface for writing
    struct KULMA_NO_VTABLE Writer
    {
        /// \brief Destructor
        virtual ~Writer() = 0;
        /// \brief Write data
        ///
        /// \param p_data Data to write
        /// \param p_size Size of data in bytes
        /// \param p_err Error handler
        /// 
        /// \return Amount of bytes written
        virtual int32_t write(const void* p_data, int32_t p_size, Error* p_err) = 0;
    };

    inline Writer::~Writer() {}

    /// \brief Interface for opening file for writing
    struct KULMA_NO_VTABLE WriterOpener
    {
        /// \brief Destructor
        virtual ~WriterOpener() = 0;
        /// \brief Opens file for writing
        /// 
        /// \param p_filePath File path
        /// \param p_err Error handler
        /// 
        /// \return True if opened successfully
        virtual bool open(const char* p_filePath, Error* p_err) = 0;
    };

    inline WriterOpener::~WriterOpener() {}

    /// \brief Interface for file reading
    struct KULMA_NO_VTABLE FileReader : public Reader, public Seeker,  public ReaderOpener, public Closer
    {

    };

    /// \brief Interface for file writing
    struct KULMA_NO_VTABLE FileWriter : public Writer, public Seeker, public WriterOpener, public Closer
    {

    };
}