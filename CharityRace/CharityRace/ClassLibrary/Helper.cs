﻿using System.ComponentModel;
using System.Diagnostics;
using System.Windows;

// Written by: Jim McCurdy (jmccurdy@facetofacesoftware.com)

namespace ClassLibrary
{
	internal static class Helper
	{
		internal static bool IsInDesignTool { get { return DesignerProperties.IsInDesignTool; } }

		// Also an Extension for string
		internal static void Trace(string message, params object[] args)
		{
			Debug.WriteLine(message, args);
		}

		// Also an Extension for string
		internal static void Alert(string message)
		{
			MessageBox.Show(message, "Alert", MessageBoxButton.OK);
		}
	}
}
