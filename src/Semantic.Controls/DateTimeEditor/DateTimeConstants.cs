using System.Collections.Generic;

namespace Semantic.Controls
{
    public static class DateTimeConstants
    {
        public static int DefaultHours = 12;
        public static int TwelveHourFormat_MaxHours = 12;
        public static int TwelveHourFormat_MaxAMHour = 11;
        public static int TwelveHourFormat_MinHours = 1;
        public static int TwentyFourHourFormat_MaxHours = 23;
        public static int TwentyFourHourFormat_MinHours = 0;
        public static int MinMinutesAndSeconds = 0;
        public static int MaxMinutesAndSeconds = 60;
        public static int TwelveHourModulus = 12;
        public static string TimeDigitsFormat = "00";
        private static List<int> _hoursTwelve = new List<int>();
        private static List<int> _hoursTwentyFour = new List<int>();
        private static List<string> _minutes = new List<string>();
        private static List<string> _seconds = new List<string>();
        private static List<AMPM> _ampms = new List<AMPM>();

        public static List<int> HoursTwelve
        {
            get
            {
                return _hoursTwelve;
            }
        }

        public static List<int> HoursTwentyFour
        {
            get
            {
                return _hoursTwentyFour;
            }
        }

        public static List<string> Minutes
        {
            get
            {
                return _minutes;
            }
        }

        public static List<string> Seconds
        {
            get
            {
                return _seconds;
            }
        }

        public static List<AMPM> AMPMs
        {
            get
            {
                return _ampms;
            }
        }

        static DateTimeConstants()
        {
            _ampms.Add(AMPM.AM);
            _ampms.Add(AMPM.PM);
            for (int index = TwelveHourFormat_MinHours; index <= TwelveHourFormat_MaxHours; ++index)
                HoursTwelve.Add(index);
            for (int index = TwentyFourHourFormat_MinHours; index <= TwentyFourHourFormat_MaxHours; ++index)
                HoursTwentyFour.Add(index);
            for (int index = MinMinutesAndSeconds; index < MaxMinutesAndSeconds; ++index)
            {
                _minutes.Add(index.ToString(TimeDigitsFormat));
                _seconds.Add(index.ToString(TimeDigitsFormat));
            }
        }

        public static int ConvertTwelveHourToTwentyFourHour(int twelveHour, AMPM ampm)
        {
            if (ampm == AMPM.PM)
            {
                if (twelveHour == TwelveHourModulus)
                    return TwelveHourModulus;
                else
                    return twelveHour + TwelveHourModulus;
            }
            else if (twelveHour == TwelveHourModulus)
                return 0;
            else
                return twelveHour;
        }

        public static int ConvertTwentyFourHourToTwelveHour(int value)
        {
            if (value == 0 || value == TwelveHourModulus)
                return TwelveHourModulus;
            else
                return value % TwelveHourModulus;
        }

        public static AMPM ConvertTwentyFourHourToAMPM(int value)
        {
            return value < 0 || value > TwelveHourFormat_MaxAMHour ? AMPM.PM : AMPM.AM;
        }
    }
}
