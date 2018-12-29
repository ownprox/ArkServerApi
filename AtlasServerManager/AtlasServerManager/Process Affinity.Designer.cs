namespace AtlasServerManager
{
    partial class Process_Affinity
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
            this.button5 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button5
            // 
            this.button5.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.button5.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold);
            this.button5.Location = new System.Drawing.Point(316, 294);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(131, 44);
            this.button5.TabIndex = 24;
            this.button5.Text = "Accept";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // Process_Affinity
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(459, 350);
            this.Controls.Add(this.button5);
            this.Name = "Process_Affinity";
            this.Text = "Process_Affinity";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button5;
    }
}