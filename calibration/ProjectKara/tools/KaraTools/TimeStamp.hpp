#pragma once

#include <chrono>
#include "Utilities.hpp"

namespace Common
{
	/** TimeStamp provides cross-platform microsecond timestamp functionality

		The TimeStamp class provides a way to store and manipulate time stamps
		at microsecond accuracy on different platforms.

		NOTE: Needs C++11 to work!
		*/
	class TimeStamp
	{
	public:
		TimeStamp(void)
		{
			SetTimeStamp(0);
		};
		TimeStamp(const uint64_t& TimeStampMicro)
		{
			SetTimeStamp(TimeStampMicro);
		};

		//! Set the data
		void SetTimeStamp(const uint64_t& TimeStampMicro) { m_TimeStampMicro = TimeStampMicro; };

		//! Set the object to current Epoch time
		void SetToCurrentEpochTime(void)
		{
			m_TimeStampMicro = getCurrentEpochTime();
		};

		//! Get time stamp in microseconds
		const uint64_t& GetTimeStamp(void) const { return m_TimeStampMicro; };

		//! Get time stamp in double
		double GetTimeStampSeconds(void) const
		{
			return double(m_TimeStampMicro) / double(1000000);
		};

		//! Get if data is valid
		bool isValid(void) const
		{
			if (m_TimeStampMicro != 0)
				return true;

			return false;
		};

	protected:
		//! This the main data source. Everything else is derived. This is in microseconds
		uint64_t m_TimeStampMicro;

		//! Check if time stamp stored is valid
		bool m_isValid;
	};

} // namespace Common
