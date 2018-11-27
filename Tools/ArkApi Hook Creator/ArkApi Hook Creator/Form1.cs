using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace ArkApi_Hook_Creator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Dictionary<int, Dictionary<string, Dictionary<int, string>>> FunctionInfo = new Dictionary<int, Dictionary<string, Dictionary<int, string>>>();
        Dictionary<string, Dictionary<int, string>> StructureSelector;
        Dictionary<int, string> FunctionSelector;

        private void AddFunction(int ClassIndex, string Structure, int FunctionIndex, string Function)
        {
            if (FunctionInfo.TryGetValue(ClassIndex, out StructureSelector))
            {
                if (StructureSelector.TryGetValue(Structure, out FunctionSelector))
                {
                    FunctionSelector.Add(FunctionIndex, Function);
                }
                else
                {
                    Dictionary<int, string> NewFuncData = new Dictionary<int, string>();
                    NewFuncData.Add(FunctionIndex, Function);
                    StructureSelector.Add(Structure, NewFuncData);
                }
            }
            else
            {
                Dictionary<string, Dictionary<int, string>> NewStructData = new Dictionary<string, Dictionary<int, string>>();
                Dictionary<int, string> NewFuncData = new Dictionary<int, string>();
                NewFuncData.Add(FunctionIndex, Function);
                NewStructData.Add(Structure, NewFuncData);
                FunctionInfo.Add(ClassIndex, NewStructData);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ClassCombo.Items.AddRange(new string[] { "Actor", "GameMode", "GameState", "Inventory", "Other", "PrimalStructure", "Tribe" });

            int ClassIndex = 0;
            foreach (string ArkHeader in ClassCombo.Items)
            {
                using (WebClient wc = new WebClient())
                {
                    int ClassId = ClassIndex++;
                    wc.DownloadStringCompleted += (object s, DownloadStringCompletedEventArgs ea) => ParseArkHeader(ClassId, s, ea);
                    wc.DownloadStringAsync(new Uri("https://raw.githubusercontent.com/Michidu/ARK-Server-API/master/version/Core/Public/API/ARK/" + ArkHeader + ".h"));
                }
            }
        }

        private void ParseArkHeader(int ClassIndex, object sender, DownloadStringCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show("Error: " + e.Error.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            int FindIndex = -1, StructureIndex = 0, FunctionIndex = 0;
            string StructName = "";
            string[] splts = Regex.Split(e.Result, "struct ");
            for (int i = 1; i < splts.Length; i++)
            {
                //Structure Name
                FindIndex = splts[i].IndexOf("\n");
                StructName = splts[i].Substring(0, FindIndex);
                if ((FindIndex = StructName.IndexOf(" :")) != -1) StructName = StructName.Substring(0, FindIndex);

                //Find Functions
                if ((FindIndex = splts[i].IndexOf("// Functions")) != -1)
                {
                    FindIndex += 15;
                    string[] splts2 = splts[i].Substring(FindIndex, splts[i].Length - FindIndex).Split('\n');
                    foreach (string s in splts2)
                    {
                        if (s.Length > 5)
                        {
                            AddFunction(ClassIndex, StructName.Replace(" * ", "* ").Replace("__declspec(align(8)) ", ""), FunctionIndex++, s.Replace("\t", ""));
                        }
                    }
                    FunctionIndex = 0;
                    StructureIndex++;
                }
            }
        }

        private void ClassCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            StructCombo.Items.Clear();
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector))
            {
                foreach (KeyValuePair<string, Dictionary<int, string>> StructShit in StructureSelector)
                {
                    StructCombo.Items.Add(StructShit.Key);
                }
            }
        }

        private void StructCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            FuncCombo.Items.Clear();
            FuncCombo.Enabled = true;
            FuncCombo.Text = "";
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector))
            {
                if (StructureSelector.TryGetValue(StructCombo.Text, out FunctionSelector))
                {
                    foreach (string FunctionName in FunctionSelector.Values)
                    {
                        if (FunctionName.Contains(" { ")) FuncCombo.Items.Add(Regex.Split(FunctionName, " { ")[0].Replace(" * ", "* "));
                        else FuncCombo.Items.Add(FunctionName.Replace(" * ", "* "));
                    }
                }
            }
        }

        private string LowerCase(string str)
        {
            if (String.IsNullOrEmpty(str)) return str;
            return Char.IsLower(str, 0) ? (Char.ToUpperInvariant(str[0]) + str.Substring(1)) : (Char.ToLowerInvariant(str[0]) + str.Substring(1));
        }

        private void FuncCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            richTextBox1.Clear();
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector))
            {
                if (StructureSelector.TryGetValue(StructCombo.Text, out FunctionSelector))
                {
                    string FunctionData;
                    if (FunctionSelector.TryGetValue(FuncCombo.SelectedIndex, out FunctionData))
                    {
                        if(FunctionData.Contains("NativeCall<"))
                        {
                            string FunctionVariables = Regex.Split(FunctionData, "NativeCall<")[1];
                            FunctionVariables = FunctionVariables.Substring(0, FunctionVariables.IndexOf('>')).Replace(" * ", "* ");
                            string FriendlyHookName = FuncCombo.Text;
                            FriendlyHookName = FriendlyHookName.Replace("* ", "").Replace("()", "").Replace(")", "");
                            if (FriendlyHookName.Contains(" ")) FriendlyHookName = FriendlyHookName.Split(' ')[1];
                            if(FriendlyHookName.Contains("(")) FriendlyHookName = FriendlyHookName.Split('(')[0];
                            string Hook = "DECLARE_HOOK(" + StructCombo.Text + "_" + FriendlyHookName;
                            string HookFunc = "";
                            if (FunctionVariables.Contains(", "))
                            {
                                string[] Vars = Regex.Split(FunctionVariables, ", ");
                                foreach (string s in Vars) Hook += ", " + s;
                                HookFunc = Vars[0] + " ";
                            }
                            else
                            {
                                Hook += ", " + FunctionVariables;
                                HookFunc = FunctionVariables + " ";
                            }
                            Hook += ");";
                            HookFunc += " Hook_" + StructCombo.Text + "_" + FriendlyHookName + "(" + StructCombo.Text + "* " + LowerCase(StructCombo.Text);
                            string Variables = FuncCombo.Text;
                            int FindIndex;
                            if((FindIndex = Variables.IndexOf('(')) != -1)
                            {
                                Variables = Variables.Remove(0, FindIndex + 1);
                                FindIndex = Variables.IndexOf(')');
                                Variables = Variables.Substring(0, FindIndex + 1);
                                HookFunc += (Variables.Length > 1 ? ", " + Variables : ")") + "\n{\n" + (HookFunc.StartsWith("void") ? "    " : "   return ");
                                HookFunc += StructCombo.Text + "_" + FriendlyHookName + "_original(" + LowerCase(StructCombo.Text);
                                if (Variables.Length > 1)
                                {
                                    string[] Vars = Regex.Split(Variables, ", ");
                                    foreach (string s in Vars)
                                    {
                                       if (s.Contains(" ")) HookFunc += ", " + s.Split(' ')[1].Replace(")", "");
                                    }
                                    HookFunc += ");\n}";
                                }
                                else HookFunc += ");\n}";
                                richTextBox1.AppendText(Hook + Environment.NewLine + Environment.NewLine + HookFunc + Environment.NewLine + Environment.NewLine
                                    + "ArkApi::GetHooks().SetHook(\"" + StructCombo.Text + "." + FriendlyHookName + "\", &Hook_" + StructCombo.Text + "_" + FriendlyHookName + ", &" + StructCombo.Text + "_" + FriendlyHookName + "_original);" + Environment.NewLine + Environment.NewLine
                                    + "ArkApi::GetHooks().DisableHook(\"" + StructCombo.Text + "." + FriendlyHookName + "\", &Hook_" + StructCombo.Text + "_" + FriendlyHookName + ");");
                                Clipboard.SetText(richTextBox1.Text);
                            }
                        }
                    }
                }
            }
        }

        private void FuncCombo_TextUpdate(object sender, EventArgs e)
        {
            if(ClassCombo.SelectedIndex == -1)
            {
                FuncCombo.Text = "";
                MessageBox.Show("Please Select a Class First!", "Class Not Selected!", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            if (StructCombo.SelectedIndex == -1)
            {
                FuncCombo.Text = "";
                MessageBox.Show("Please Select a Structure First!", "Structure Not Selected!", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return;
            }
            FuncCombo.Items.Clear();
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector))
            {
                if (StructureSelector.TryGetValue(StructCombo.Text, out FunctionSelector))
                {
                    string FuncName;
                    foreach (string FunctionName in FunctionSelector.Values)
                    {
                        if (FunctionName.Contains(" { "))
                        {
                            FuncName = Regex.Split(FunctionName, " { ")[0].Replace(" * ", "* ");
                            if (FuncName.ToLower().Contains(FuncCombo.Text.ToLower()))
                                FuncCombo.Items.Add(FuncName);
                        }
                        else
                        {
                            FuncName = FunctionName.Replace(" * ", "* ");
                            if (FuncName.ToLower().Contains(FuncCombo.Text.ToLower()))
                                FuncCombo.Items.Add(FuncName);
                        }
                    }
                }
            }
            FuncCombo.SelectionStart = FuncCombo.Text.Length;
            FuncCombo.SelectionLength = 0;
        }
    }
}