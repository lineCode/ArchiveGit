﻿#pragma checksum "C:\My Projects\Trump\ttnweb_trunk\Silverlight\ClassLibrary\MessageBoxDialog.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "FC5083C7627E588FB6BF7F0349F95138"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.4927
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using ClassLibrary;
using System;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Automation.Peers;
using System.Windows.Automation.Provider;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using System.Windows.Shapes;
using System.Windows.Threading;


namespace ClassLibrary {
    
    
    public partial class MessageBoxDialog : ClassLibrary.DialogPanel {
        
        internal System.Windows.Controls.TextBlock x_Title;
        
        internal System.Windows.Controls.Image x_Icon;
        
        internal System.Windows.Controls.TextBlock x_Text;
        
        internal System.Windows.Controls.Button x_Yes;
        
        internal System.Windows.Controls.Button x_No;
        
        internal System.Windows.Controls.Button x_OK;
        
        internal System.Windows.Controls.Button x_Cancel;
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Windows.Application.LoadComponent(this, new System.Uri("/ClassLibrary;component/MessageBoxDialog.xaml", System.UriKind.Relative));
            this.x_Title = ((System.Windows.Controls.TextBlock)(this.FindName("x_Title")));
            this.x_Icon = ((System.Windows.Controls.Image)(this.FindName("x_Icon")));
            this.x_Text = ((System.Windows.Controls.TextBlock)(this.FindName("x_Text")));
            this.x_Yes = ((System.Windows.Controls.Button)(this.FindName("x_Yes")));
            this.x_No = ((System.Windows.Controls.Button)(this.FindName("x_No")));
            this.x_OK = ((System.Windows.Controls.Button)(this.FindName("x_OK")));
            this.x_Cancel = ((System.Windows.Controls.Button)(this.FindName("x_Cancel")));
        }
    }
}

