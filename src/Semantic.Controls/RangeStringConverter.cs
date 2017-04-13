using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Windows;
using System.Windows.Data;

namespace Semantic.Controls
{
    public class RangeStringConverter : IMultiValueConverter
    {
        public static double? RoundRangeValue(double? value, double? Minimum, double? Maximum, bool noDecimals)
        {
            if (!value.HasValue)
                return new double?();
            if (!Maximum.HasValue || !Minimum.HasValue)
                return value;
            double? nullable1 = value;
            double? nullable2 = Maximum;
            if ((nullable1.GetValueOrDefault() != nullable2.GetValueOrDefault() ? 0 : (nullable1.HasValue == nullable2.HasValue ? 1 : 0)) == 0)
            {
                double? nullable3 = value;
                double? nullable4 = Minimum;
                if ((nullable3.GetValueOrDefault() != nullable4.GetValueOrDefault() ? 0 : (nullable3.HasValue == nullable4.HasValue ? 1 : 0)) == 0)
                {
                    double num1 = value.Value;
                    try
                    {
                        if (noDecimals)
                        {
                            num1 = Math.Round(value.Value, MidpointRounding.AwayFromZero);
                        }
                        else
                        {
                            int num2 = (int)Math.Ceiling(2.0 - Math.Log10(Maximum.Value - Minimum.Value));
                            num1 = Math.Round(value.Value, num2 < 0 ? 0 : num2, MidpointRounding.AwayFromZero);
                            double num3 = num1;
                            double? nullable5 = Maximum;
                            if ((num3 <= nullable5.GetValueOrDefault() ? 0 : (nullable5.HasValue ? 1 : 0)) != 0)
                                num1 = Maximum.Value;
                            double num4 = num1;
                            double? nullable6 = Minimum;
                            if ((num4 >= nullable6.GetValueOrDefault() ? 0 : (nullable6.HasValue ? 1 : 0)) != 0)
                                num1 = Minimum.Value;
                        }
                    }
                    catch
                    {
                    }
                    return new double?(num1);
                }
            }
            return value;
        }

        public object Convert(object[] values, Type targetType, object parameter, CultureInfo culture)
        {
            if (Enumerable.Any<object>(values, val => val == DependencyProperty.UnsetValue) ||
                Enumerable.Any<object>(Enumerable.Skip<object>(values, 1), val => val == null))
                return values[0];
            string str = (string)values[0];
            double num1 = (double)values[1];
            double num2 = (double)values[2];
            double num3 = (double)values[3];
            double num4 = (double)values[4];
            bool flag1 = (bool)values[5];
            bool flag2 = (bool)values[6];
            if (num1 == num3 && num2 == num4 && flag2)
                return str;
            double? val1 = RoundRangeValue(new double?(num3), new double?(num1), new double?(num2), flag1);
            double? val2 = RoundRangeValue(new double?(num4), new double?(num1), new double?(num2), flag1);
            if (num3 == num4)
                return string.Format(CultureInfo.CurrentUICulture, "= {0}", new object[1]
                {
          GetValue(val1, flag1)
                });
            else
                return string.Format(CultureInfo.CurrentUICulture, "在 {0} 和 {1} 之间", new object[2]
                {
          GetValue(val1, flag1),
          GetValue(val2, flag1)
                });
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }

        private static string GetValue(double? val, bool isInteger)
        {
            if (!val.HasValue)
                return null;
            string format = Math.Abs(val.Value) <= 0.01 || Math.Abs(val.Value) >= 1E+21 ? null : (isInteger ? "N0" : "N");
            return val.Value.ToString(format, CultureInfo.CurrentUICulture);
        }
    }
}
