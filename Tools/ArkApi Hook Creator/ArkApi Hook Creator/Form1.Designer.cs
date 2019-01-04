namespace TrampolineCreator
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.StructCombo = new System.Windows.Forms.ComboBox();
            this.FuncCombo = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.ClassCombo = new System.Windows.Forms.ComboBox();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.GameCombo = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // StructCombo
            // 
            this.StructCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.StructCombo.Enabled = false;
            this.StructCombo.FormattingEnabled = true;
            this.StructCombo.Location = new System.Drawing.Point(658, 6);
            this.StructCombo.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.StructCombo.Name = "StructCombo";
            this.StructCombo.Size = new System.Drawing.Size(415, 31);
            this.StructCombo.TabIndex = 1;
            this.StructCombo.SelectedIndexChanged += new System.EventHandler(this.StructCombo_SelectedIndexChanged);
            // 
            // FuncCombo
            // 
            this.FuncCombo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.FuncCombo.Enabled = false;
            this.FuncCombo.FormattingEnabled = true;
            this.FuncCombo.Location = new System.Drawing.Point(1210, 6);
            this.FuncCombo.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.FuncCombo.Name = "FuncCombo";
            this.FuncCombo.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.FuncCombo.Size = new System.Drawing.Size(371, 31);
            this.FuncCombo.TabIndex = 2;
            this.FuncCombo.SelectedIndexChanged += new System.EventHandler(this.FuncCombo_SelectedIndexChanged);
            this.FuncCombo.TextUpdate += new System.EventHandler(this.FuncCombo_TextUpdate);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(551, 9);
            this.label1.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(106, 23);
            this.label1.TabIndex = 2;
            this.label1.Text = "Structure:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(1110, 9);
            this.label2.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(100, 23);
            this.label2.TabIndex = 3;
            this.label2.Text = "Function:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(206, 9);
            this.label3.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(67, 23);
            this.label3.TabIndex = 2;
            this.label3.Text = "Class:";
            // 
            // ClassCombo
            // 
            this.ClassCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ClassCombo.Enabled = false;
            this.ClassCombo.FormattingEnabled = true;
            this.ClassCombo.Location = new System.Drawing.Point(273, 6);
            this.ClassCombo.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.ClassCombo.Name = "ClassCombo";
            this.ClassCombo.Size = new System.Drawing.Size(238, 31);
            this.ClassCombo.TabIndex = 0;
            this.ClassCombo.SelectedIndexChanged += new System.EventHandler(this.ClassCombo_SelectedIndexChanged);
            // 
            // richTextBox1
            // 
            this.richTextBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.richTextBox1.Location = new System.Drawing.Point(0, 45);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(1608, 606);
            this.richTextBox1.TabIndex = 3;
            this.richTextBox1.Text = "";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(-1, 9);
            this.label4.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 23);
            this.label4.TabIndex = 5;
            this.label4.Text = "Game:";
            // 
            // GameCombo
            // 
            this.GameCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.GameCombo.Enabled = false;
            this.GameCombo.FormattingEnabled = true;
            this.GameCombo.Items.AddRange(new object[] {
            "Ark",
            "Atlas"});
            this.GameCombo.Location = new System.Drawing.Point(70, 6);
            this.GameCombo.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.GameCombo.Name = "GameCombo";
            this.GameCombo.Size = new System.Drawing.Size(103, 31);
            this.GameCombo.TabIndex = 4;
            this.GameCombo.SelectedIndexChanged += new System.EventHandler(this.GameCombo_SelectedIndexChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 23F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1608, 652);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.GameCombo);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.ClassCombo);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.FuncCombo);
            this.Controls.Add(this.StructCombo);
            this.Font = new System.Drawing.Font("Tahoma", 14.25F, System.Drawing.FontStyle.Bold);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(6, 5, 6, 5);
            this.Name = "Form1";
            this.Text = "Server Api Hooker Creator v1.6";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox StructCombo;
        private System.Windows.Forms.ComboBox FuncCombo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox ClassCombo;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox GameCombo;
    }
}

