using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
namespace AtlasServerManager.Includes
{
    public class ArkListView : ListView
    {
        public List<AtlasServerData> GetServerList()
        {
            List<AtlasServerData> ServerList = new List<AtlasServerData>();
            foreach (ArkServerListViewItem itm in Items) ServerList.Add(itm.GetServerData());
            return ServerList;
        }
    }
 
    public class ArkServerListViewItem : ListViewItem
    {
        private AtlasServerData ServerData;
        public void SetServerData(AtlasServerData ServerData) { this.ServerData = ServerData; UpdateSubItemText(); }
        public AtlasServerData GetServerData() { return ServerData; }
        public ArkServerListViewItem(AtlasServerData ServerData) 
        {
            Checked = true;
            SetServerData(ServerData);
            UseItemStyleForSubItems = false;
            Text = ServerData.ServerPort.ToString();
            SubItems.AddRange(new string[] { ServerData.AltSaveDirectory, ServerData.ServerX.ToString().ToString(), ServerData.ServerY.ToString(), "Offline", "0" });
            SubItems[0].ForeColor = Color.LightSlateGray;
            SubItems[1].ForeColor = Color.LightSkyBlue;
            SubItems[2].ForeColor = Color.YellowGreen;
            SubItems[3].ForeColor = Color.LightSeaGreen;
            SubItems[4].ForeColor = Color.LightCoral;
            SubItems[5].ForeColor = Color.LightSlateGray;
            UpdateStatus();
        }

        private void UpdateSubItemText()
        {
            if (SubItems.Count > 1)
            {
                Text = ServerData.AltSaveDirectory;
                SubItems[1].Text = ServerData.ServerPort.ToString();
                SubItems[2].Text = ServerData.ServerX.ToString().ToString();
                SubItems[3].Text = ServerData.ServerY.ToString();
            }
        }

        public void UpdateStatus()
        {
            if (ServerData.IsRunning())
            {
                SubItems[4].ForeColor = Color.Green;
                SubItems[4].Text = "Online";
            }
            else
            {
                SubItems[4].ForeColor = Color.Red;
                SubItems[4].Text = "Offline";
            }
        }
    }
}