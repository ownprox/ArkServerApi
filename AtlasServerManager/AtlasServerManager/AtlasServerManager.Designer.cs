namespace AtlasServerManager
{
    partial class AtlasServerManager
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AtlasServerManager));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.button1 = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.numServerMonitor = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.checkBootWhenOff = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.numServerUpdate = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.numServerWarning = new System.Windows.Forms.NumericUpDown();
            this.checkAutoServerUpdate = new System.Windows.Forms.CheckBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.numAutoUpdateMod = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.numAutoUpdateModWarning = new System.Windows.Forms.NumericUpDown();
            this.checkAutoModUpdate = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.serverToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editSettingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.manageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.startToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.ServerList = new Includes.ArkListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numServerMonitor)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numServerUpdate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numServerWarning)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numAutoUpdateMod)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numAutoUpdateModWarning)).BeginInit();
            this.tabPage1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.button1);
            this.tabPage2.Controls.Add(this.groupBox1);
            this.tabPage2.Controls.Add(this.groupBox3);
            this.tabPage2.Controls.Add(this.groupBox2);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(4);
            this.tabPage2.Size = new System.Drawing.Size(696, 318);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Options";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(298, 115);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(266, 99);
            this.button1.TabIndex = 7;
            this.button1.Text = "Force Update";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.numServerMonitor);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.checkBootWhenOff);
            this.groupBox1.Location = new System.Drawing.Point(298, 7);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(266, 75);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Automatic Ark Server Monitor";
            // 
            // numServerMonitor
            // 
            this.numServerMonitor.Location = new System.Drawing.Point(209, 45);
            this.numServerMonitor.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
            this.numServerMonitor.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numServerMonitor.Name = "numServerMonitor";
            this.numServerMonitor.Size = new System.Drawing.Size(51, 24);
            this.numServerMonitor.TabIndex = 4;
            this.numServerMonitor.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Location = new System.Drawing.Point(7, 47);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(176, 17);
            this.label1.TabIndex = 3;
            this.label1.Text = "Check Every (Seconds): ";
            // 
            // checkBootWhenOff
            // 
            this.checkBootWhenOff.AutoSize = true;
            this.checkBootWhenOff.Checked = true;
            this.checkBootWhenOff.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBootWhenOff.Location = new System.Drawing.Point(10, 23);
            this.checkBootWhenOff.Name = "checkBootWhenOff";
            this.checkBootWhenOff.Size = new System.Drawing.Size(202, 21);
            this.checkBootWhenOff.TabIndex = 2;
            this.checkBootWhenOff.Text = "Boot Server When Offline";
            this.checkBootWhenOff.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.numServerUpdate);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.numServerWarning);
            this.groupBox3.Controls.Add(this.checkAutoServerUpdate);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Location = new System.Drawing.Point(7, 7);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(266, 102);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Ark Server Automatic Update";
            // 
            // numServerUpdate
            // 
            this.numServerUpdate.Location = new System.Drawing.Point(205, 49);
            this.numServerUpdate.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
            this.numServerUpdate.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numServerUpdate.Name = "numServerUpdate";
            this.numServerUpdate.Size = new System.Drawing.Size(51, 24);
            this.numServerUpdate.TabIndex = 4;
            this.numServerUpdate.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.ForeColor = System.Drawing.Color.Black;
            this.label4.Location = new System.Drawing.Point(6, 51);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(172, 17);
            this.label4.TabIndex = 3;
            this.label4.Text = "Check Every (Minutes): ";
            // 
            // numServerWarning
            // 
            this.numServerWarning.Enabled = false;
            this.numServerWarning.Location = new System.Drawing.Point(205, 72);
            this.numServerWarning.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
            this.numServerWarning.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.numServerWarning.Name = "numServerWarning";
            this.numServerWarning.Size = new System.Drawing.Size(51, 24);
            this.numServerWarning.TabIndex = 1;
            this.numServerWarning.Value = new decimal(new int[] {
            30,
            0,
            0,
            0});
            // 
            // checkAutoServerUpdate
            // 
            this.checkAutoServerUpdate.AutoSize = true;
            this.checkAutoServerUpdate.Checked = true;
            this.checkAutoServerUpdate.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkAutoServerUpdate.Location = new System.Drawing.Point(6, 23);
            this.checkAutoServerUpdate.Name = "checkAutoServerUpdate";
            this.checkAutoServerUpdate.Size = new System.Drawing.Size(223, 21);
            this.checkAutoServerUpdate.TabIndex = 2;
            this.checkAutoServerUpdate.Text = "Automatically Update Server";
            this.checkAutoServerUpdate.UseVisualStyleBackColor = true;
            this.checkAutoServerUpdate.CheckedChanged += new System.EventHandler(this.checkAutoServerUpdate_CheckedChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Enabled = false;
            this.label5.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.ForeColor = System.Drawing.Color.Black;
            this.label5.Location = new System.Drawing.Point(6, 74);
            this.label5.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(199, 17);
            this.label5.TabIndex = 0;
            this.label5.Text = "Update Warning (Minutes): ";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.numAutoUpdateMod);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.numAutoUpdateModWarning);
            this.groupBox2.Controls.Add(this.checkAutoModUpdate);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Enabled = false;
            this.groupBox2.Location = new System.Drawing.Point(7, 115);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(266, 99);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Automatic Mod Updates";
            // 
            // numAutoUpdateMod
            // 
            this.numAutoUpdateMod.Enabled = false;
            this.numAutoUpdateMod.Location = new System.Drawing.Point(209, 46);
            this.numAutoUpdateMod.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
            this.numAutoUpdateMod.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numAutoUpdateMod.Name = "numAutoUpdateMod";
            this.numAutoUpdateMod.Size = new System.Drawing.Size(51, 24);
            this.numAutoUpdateMod.TabIndex = 4;
            this.numAutoUpdateMod.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.ForeColor = System.Drawing.Color.Black;
            this.label3.Location = new System.Drawing.Point(7, 48);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(172, 17);
            this.label3.TabIndex = 3;
            this.label3.Text = "Check Every (Minutes): ";
            // 
            // numAutoUpdateModWarning
            // 
            this.numAutoUpdateModWarning.Enabled = false;
            this.numAutoUpdateModWarning.Location = new System.Drawing.Point(209, 69);
            this.numAutoUpdateModWarning.Maximum = new decimal(new int[] {
            99999,
            0,
            0,
            0});
            this.numAutoUpdateModWarning.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.numAutoUpdateModWarning.Name = "numAutoUpdateModWarning";
            this.numAutoUpdateModWarning.Size = new System.Drawing.Size(51, 24);
            this.numAutoUpdateModWarning.TabIndex = 1;
            this.numAutoUpdateModWarning.Value = new decimal(new int[] {
            15,
            0,
            0,
            0});
            // 
            // checkAutoModUpdate
            // 
            this.checkAutoModUpdate.AutoSize = true;
            this.checkAutoModUpdate.Checked = true;
            this.checkAutoModUpdate.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkAutoModUpdate.Enabled = false;
            this.checkAutoModUpdate.Location = new System.Drawing.Point(10, 23);
            this.checkAutoModUpdate.Name = "checkAutoModUpdate";
            this.checkAutoModUpdate.Size = new System.Drawing.Size(215, 21);
            this.checkAutoModUpdate.TabIndex = 2;
            this.checkAutoModUpdate.Text = "Automatically Update Mods";
            this.checkAutoModUpdate.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.Black;
            this.label2.Location = new System.Drawing.Point(7, 71);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(199, 17);
            this.label2.TabIndex = 0;
            this.label2.Text = "Update Warning (Minutes): ";
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.ServerList);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(4);
            this.tabPage1.Size = new System.Drawing.Size(696, 318);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Servers";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.serverToolStripMenuItem,
            this.manageToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(118, 48);
            // 
            // serverToolStripMenuItem
            // 
            this.serverToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.removeToolStripMenuItem,
            this.editSettingsToolStripMenuItem});
            this.serverToolStripMenuItem.Name = "serverToolStripMenuItem";
            this.serverToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.serverToolStripMenuItem.Text = "Server";
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(139, 22);
            this.addToolStripMenuItem.Text = "Add";
            this.addToolStripMenuItem.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
            // 
            // removeToolStripMenuItem
            // 
            this.removeToolStripMenuItem.Name = "removeToolStripMenuItem";
            this.removeToolStripMenuItem.Size = new System.Drawing.Size(139, 22);
            this.removeToolStripMenuItem.Text = "Remove";
            this.removeToolStripMenuItem.Click += new System.EventHandler(this.removeToolStripMenuItem_Click);
            // 
            // editSettingsToolStripMenuItem
            // 
            this.editSettingsToolStripMenuItem.Name = "editSettingsToolStripMenuItem";
            this.editSettingsToolStripMenuItem.Size = new System.Drawing.Size(139, 22);
            this.editSettingsToolStripMenuItem.Text = "Edit Settings";
            this.editSettingsToolStripMenuItem.Click += new System.EventHandler(this.editSettingsToolStripMenuItem_Click);
            // 
            // manageToolStripMenuItem
            // 
            this.manageToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.startToolStripMenuItem,
            this.stopToolStripMenuItem});
            this.manageToolStripMenuItem.Name = "manageToolStripMenuItem";
            this.manageToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.manageToolStripMenuItem.Text = "Manage";
            // 
            // startToolStripMenuItem
            // 
            this.startToolStripMenuItem.Name = "startToolStripMenuItem";
            this.startToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.startToolStripMenuItem.Text = "Start";
            this.startToolStripMenuItem.Click += new System.EventHandler(this.startToolStripMenuItem_Click);
            // 
            // stopToolStripMenuItem
            // 
            this.stopToolStripMenuItem.Name = "stopToolStripMenuItem";
            this.stopToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.stopToolStripMenuItem.Text = "Stop";
            this.stopToolStripMenuItem.Click += new System.EventHandler(this.stopToolStripMenuItem_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(1, 3);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(4);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(704, 347);
            this.tabControl1.TabIndex = 0;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox1.Location = new System.Drawing.Point(0, 0);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(702, 129);
            this.richTextBox1.TabIndex = 1;
            this.richTextBox1.Text = "";
            // 
            // toolTip1
            // 
            this.toolTip1.ToolTipIcon = System.Windows.Forms.ToolTipIcon.Info;
            this.toolTip1.ToolTipTitle = "Right Click Here!!!!!";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.richTextBox1);
            this.splitContainer1.Size = new System.Drawing.Size(702, 483);
            this.splitContainer1.SplitterDistance = 350;
            this.splitContainer1.TabIndex = 2;
            // 
            // ServerList
            // 
            this.ServerList.CheckBoxes = true;
            this.ServerList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader6,
            this.columnHeader7,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader2,
            this.columnHeader3});
            this.ServerList.ContextMenuStrip = this.contextMenuStrip1;
            this.ServerList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ServerList.FullRowSelect = true;
            this.ServerList.GridLines = true;
            this.ServerList.Location = new System.Drawing.Point(4, 4);
            this.ServerList.Margin = new System.Windows.Forms.Padding(4);
            this.ServerList.Name = "ServerList";
            this.ServerList.Size = new System.Drawing.Size(688, 310);
            this.ServerList.TabIndex = 0;
            this.ServerList.UseCompatibleStateImageBehavior = false;
            this.ServerList.View = System.Windows.Forms.View.Details;
            this.ServerList.SelectedIndexChanged += new System.EventHandler(this.ServerList_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Server Port";
            this.columnHeader1.Width = 154;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "ServerX";
            this.columnHeader6.Width = 80;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "ServerY";
            this.columnHeader7.Width = 80;
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Status";
            this.columnHeader4.Width = 80;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Players Online";
            this.columnHeader5.Width = 107;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "CPU";
            this.columnHeader2.Width = 54;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "AltSaveDir";
            this.columnHeader3.Width = 129;
            // 
            // AtlasServerManager
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(702, 483);
            this.Controls.Add(this.splitContainer1);
            this.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "AtlasServerManager";
            this.Text = "Atlas Server Manager 1.2";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabPage2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numServerMonitor)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numServerUpdate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numServerWarning)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numAutoUpdateMod)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numAutoUpdateModWarning)).EndInit();
            this.tabPage1.ResumeLayout(false);
            this.contextMenuStrip1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem manageToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem startToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem stopToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ToolStripMenuItem serverToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editSettingsToolStripMenuItem;
        public Includes.ArkListView ServerList;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        public System.Windows.Forms.NumericUpDown numServerMonitor;
        public System.Windows.Forms.CheckBox checkBootWhenOff;
        public System.Windows.Forms.NumericUpDown numServerUpdate;
        public System.Windows.Forms.NumericUpDown numServerWarning;
        public System.Windows.Forms.CheckBox checkAutoServerUpdate;
        public System.Windows.Forms.NumericUpDown numAutoUpdateMod;
        public System.Windows.Forms.NumericUpDown numAutoUpdateModWarning;
        public System.Windows.Forms.CheckBox checkAutoModUpdate;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.SplitContainer splitContainer1;
    }
}

