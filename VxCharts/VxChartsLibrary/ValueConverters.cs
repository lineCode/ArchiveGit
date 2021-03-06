﻿using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Windows.Data;
using System.Collections.ObjectModel;

namespace ClassLibrary
{
	public class StringFormatter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			return string.Format(culture, (parameter as string) ?? "{0}", value);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class DateTimeToLongString : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!(value is DateTime))
				throw new Exception(string.Format("'{0}' is not a DateTime", value));

			DateTime dateTime = (DateTime)value;
			return dateTime.ToLongDateString();
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class BoolToOpacity : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			bool hidden = (bool)value;
			return hidden ? 0.5 : 1.0;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			double opacity = (double)value;
			return (opacity != 1.0);
		}
	}

	public class BoolToVisible : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			bool visibile = (bool)value;
			return visibile ? Visibility.Visible : Visibility.Collapsed;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			Visibility visibility = (Visibility)value;
			return (visibility == Visibility.Visible);
		}
	}

	public class BoolToCollapsed : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			bool collapsed = (bool)value;
			return collapsed ? Visibility.Collapsed : Visibility.Visible;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			Visibility visibility = (Visibility)value;
			return (visibility == Visibility.Collapsed);
		}
	}

	public class ValueIsNonZeroToVisible : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", value));

			double dvalue = System.Convert.ToDouble(value);
			return (dvalue != 0 ? Visibility.Visible : Visibility.Collapsed);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class ValueIsNonZeroToCollapsed : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", value));

			double dvalue = System.Convert.ToDouble(value);
			return (dvalue != 0 ? Visibility.Collapsed : Visibility.Visible);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class ValueIsZero : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", value));

			double dvalue = System.Convert.ToDouble(value);
			return (dvalue == 0);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;

			return 0;
		}
	}

	public class ValueIsNonZero : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", value));

			double dvalue = System.Convert.ToDouble(value);
			return (dvalue != 0);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;

			return 0;
		}
	}

	public class ValueIsNonNegative : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", value));

			double dvalue = System.Convert.ToDouble(value);
			return (dvalue >= 0);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class ValueToString : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			return value.ToString().ExpandMixedCase();
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class EnumToValue : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsEnum)
	            throw new Exception(string.Format("'{0}' is not an enum", value));
			if (!targetType.IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", targetType));

			return System.Convert.ToInt32(value);
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsValueType)
				throw new Exception(string.Format("'{0}' is not a value", value));
			if (!targetType.IsEnum)
				throw new Exception(string.Format("'{0}' is not an enum", targetType));

			// During data binding de-referencing, -1 values are passed in, 
			// and we have no choice but to return a null value
			if (!Enum.IsDefined(targetType, value))
				return null;

			return Enum.ToObject(targetType, value);
		}
	}

	public class EnumToName : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsEnum)
	            throw new Exception(string.Format("'{0}' is not an enum", value));

			return value.ToString().ExpandMixedCase();
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!(value is string))
				throw new Exception(string.Format("'{0}' is not a string", value));
			if (!targetType.IsEnum)
				throw new Exception(string.Format("'{0}' is not an enum", targetType));

			string name = ((string)value).Replace(" ", null);
			if (!Enum.IsDefined(targetType, name))
				return null;

			return Enum.Parse(targetType, name, true/*ignoreCase*/);
		}
	}

	public class EnumToList : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			if (!value.GetType().IsEnum)
	            throw new Exception(string.Format("'{0}' is not an enum", value));

			Collection<string> enumList = new Collection<string>();
			FieldInfo[] fieldInfo = value.GetType().GetFields();
			foreach (FieldInfo field in fieldInfo)
			{
				if (!field.IsLiteral)
					continue;
				object fieldValue = field.GetValue(value.GetType());
				int enumValue = System.Convert.ToInt32(fieldValue);
				// As convention, don't put negative enum values in the list so we can "hide" them
				if (enumValue < 0)
					continue;
				string name = fieldValue.ToString().ExpandMixedCase();
				enumList.Add(name);
			}

			return enumList;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			throw new NotSupportedException();
		}
	}

	public class DoubleRound : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			double dvalue = (double)value;
			return dvalue.Round();
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			return value;
		}
	}

	public class IconNameToImageUri : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;
			string iconName = (string)value;
			string imageName = iconName + ".png";
			string imageUri = UriHelper.UriImages(imageName).ToString();
			return imageUri;
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null) // Special case
				return null;

			string imageUri = (string)value;
			string iconName = Path.GetFileNameWithoutExtension(imageUri);
			return iconName;
		}
	}
}
