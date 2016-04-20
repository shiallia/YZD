using System;
//using System.Collections.Generic;
//using System.Diagnostics;
//using System.Linq;
//using System.Management;
//using System.Net;
//using System.Net.NetworkInformation;
//using System.Text;
using System.Windows;
//using System.Windows.Controls;
//using System.Windows.Data;
//using System.Windows.Documents;
//using System.Windows.Input;
//using System.Windows.Media;
//using System.Windows.Media.Imaging;
//using System.Windows.Navigation;
//using System.Windows.Shapes;

namespace mgen_memcpu
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ulong totMem;
        Utility util;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            util = new Utility();
            totMem = util.GetPhysicalMemSize();
            System.Threading.ThreadPool.QueueUserWorkItem(WorkerItem, new Action<float, ulong, string, string>(Callback));
            memTotal.Text = ConvertBytes(totMem, 3) + " GB";
        }

        void WorkerItem(object arg)
        {
            var callback = (Action<float, ulong, string, string>)arg;
            util.Monitor(callback);
        }

        void Callback(float cpu, ulong mem , string ip , string yzdmem)
        {
            var pcpu = Math.Round(cpu, 2, MidpointRounding.AwayFromZero);
            var used_mem=totMem-mem;
            var mb_mem = ConvertBytes(used_mem, 2);
            var pmem = ConvertBytes(Convert.ToUInt64(mb_mem), 1);

            Dispatcher.Invoke((Action)(() =>
                {
                    cpuT.Text = pcpu + "%";
                    cpuP.Value = pcpu;
                    memT.Text = String.Format("内存总占用：{0} GB ({1:N} MB)", pmem, mb_mem);
                    yzdmemT.Text = "医诊达程序占用："+yzdmem+"MB"; 
                    memP.Value = (double)(used_mem / Convert.ToDecimal(totMem) * 100);
                    ipT.Text = ip;
                }));
        }

        decimal ConvertBytes(ulong b, int iteration)
        {
            long iter = 1;
            for (int i = 0; i < iteration; i++)
                iter *= 1024;
            return Math.Round((Convert.ToDecimal(b)) / Convert.ToDecimal(iter), 2, MidpointRounding.AwayFromZero);
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            util.Dispose();
        }
    }
}
