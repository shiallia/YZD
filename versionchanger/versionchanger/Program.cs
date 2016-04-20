using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
//using System.Threading.Tasks;

namespace versionchanger
{
    class Program
    {
        static void Main(string[] args)
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load("d://yzd//updateconfiguration.config");
           
            XmlNode node = xmlDoc.SelectSingleNode("applicationUpdater");
            XmlElement xe = (XmlElement)node;
            xe.SetAttribute("version", "0");
            xmlDoc.Save("d://yzd//updateconfiguration.config");

        }
    }
}
