using System;
using System.Collections.Generic;
using System.Net;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace TrampolineCreator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        bool LoadingHeaders = false;
        Dictionary<int, Dictionary<string, Dictionary<int, string>>> FunctionInfo = new Dictionary<int, Dictionary<string, Dictionary<int, string>>>();
        Dictionary<string, Dictionary<int, string>> StructureSelector = new Dictionary<string, Dictionary<int, string>>();
        Dictionary<int, string> FunctionSelector = new Dictionary<int, string>();
        Dictionary<int, int> FunctionIndexer = new Dictionary<int, int>();

        private void AddFunction(int ClassIndex, string Structure, int FunctionIndex, string Function)
        {
            if (FunctionInfo.TryGetValue(ClassIndex, out StructureSelector))
            {
                if (StructureSelector.TryGetValue(Structure, out FunctionSelector))
                {
                    if(FunctionSelector.ContainsKey(FunctionIndex))
                        return;
                    FunctionSelector.Add(FunctionIndex, Function);
                }
                else StructureSelector.Add(Structure, new Dictionary<int, string> { { FunctionIndex, Function } });
            }
            else FunctionInfo.Add(ClassIndex, new Dictionary<string, Dictionary<int, string>> { { Structure, new Dictionary<int, string> { { FunctionIndex, Function } } } });
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            GameCombo.SelectedIndex = 0;
        }

        private void GameCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            LoadHeaders();
        }

        private void LoadHeaders()
        {
            if (LoadingHeaders) return;
            LoadingHeaders = true;
            if (ClassCombo.Items.Count > 0)
            {
                ClassCombo.Items.Clear();
                StructCombo.Items.Clear();
                FuncCombo.Items.Clear();
                FunctionInfo.Clear();
                StructureSelector.Clear();
                FunctionSelector.Clear();
                FunctionIndexer.Clear();
                GameCombo.Enabled = ClassCombo.Enabled = StructCombo.Enabled = false;
            }
            ClassCombo.Items.AddRange(new string[] { "Actor", "GameMode", "GameState", "Inventory", "Other", "PrimalStructure", "Tribe" });
            int ClassIndex = 0, ClassCount = ClassCombo.Items.Count - 1;
            foreach (string ArkHeader in ClassCombo.Items)
            {
                using (WebClient wc = new WebClient())
                {
                    int ClassId = ClassIndex++;
                    wc.DownloadStringCompleted += (object s, DownloadStringCompletedEventArgs ea) => ParseArkHeader(ClassId, ClassId == ClassCount, s, ea);
                    wc.DownloadStringAsync(new Uri("https://raw.githubusercontent.com/Michidu/ARK-Server-API/master/version/Core/Public/API/" + (GameCombo.SelectedIndex == 0 ? "ARK" : "Atlas") + "/" + ArkHeader + ".h"));
                }
            }
        }

        private void ParseArkHeader(int ClassIndex, bool Completed, object sender, DownloadStringCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show("Error: " + e.Error.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            string HtmlData = e.Result.Replace("TWeakObjectPtr<struct ", "TWeakObjectPtr<");
            int FindIndex = -1, StructureIndex = 0, FunctionIndex = 0, indexof = HtmlData.IndexOf("	struct ");
            //Remove structures within structures
            if (indexof != -1)
            {
                int indexofend = HtmlData.IndexOf('}', indexof);
                while(indexof != -1)
                {
                    HtmlData = HtmlData.Remove(indexof, indexofend - indexof + 2);
                    indexof = HtmlData.IndexOf("	struct ");
                    if (indexof != -1) indexofend = HtmlData.IndexOf('}', indexof);
                }
            }
            
            string StructName = "";
            string[] splts = Regex.Split(HtmlData, "struct ");
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
                    string[] Functions = splts[i].Substring(FindIndex, splts[i].Length - FindIndex).Split('\n');
                    foreach (string Function in Functions)
                    {
                        if (Function.StartsWith("	//")) continue;
                        if (Function.Length > 5) AddFunction(ClassIndex, StructName.Replace(" * ", "* ").Replace("__declspec(align(8)) ", ""), FunctionIndex++, Function.Replace("\t", ""));
                    }
                    FunctionIndex = 0;
                    StructureIndex++;
                }
            }

            if (Completed)
            {
                GameCombo.Enabled = ClassCombo.Enabled = StructCombo.Enabled = true;
                LoadingHeaders = false;
            }
        }

        private void ClassCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            StructCombo.Items.Clear();
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector))
                foreach (KeyValuePair<string, Dictionary<int, string>> StructShit in StructureSelector)
                    StructCombo.Items.Add(StructShit.Key);
        }

        private void StructCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            FunctionIndexer.Clear();
            FuncCombo.Items.Clear();
            FuncCombo.Enabled = true;
            FuncCombo.Text = "";
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector) && StructureSelector.TryGetValue(StructCombo.Text, out FunctionSelector))
                foreach (KeyValuePair<int, string> func in FunctionSelector)
                {
                    FunctionIndexer.Add(FuncCombo.Items.Count, func.Key);
                    if (func.Value.Contains(" { ")) FuncCombo.Items.Add(Regex.Split(func.Value, " { ")[0].Replace(" * ", "* "));
                    else FuncCombo.Items.Add(func.Value.Replace(" * ", "* "));
                }
        }

        private string LowerCase(string str)
        {
            return string.IsNullOrEmpty(str) ? str : char.IsLower(str, 0) ? (char.ToUpperInvariant(str[0]) + str.Substring(1)) : (char.ToLowerInvariant(str[0]) + str.Substring(1));
        }

        private void FuncCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            richTextBox1.Clear();
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector) && StructureSelector.TryGetValue(StructCombo.Text, out FunctionSelector) 
            && FunctionIndexer.TryGetValue(FuncCombo.SelectedIndex, out int FuncIndex) && FunctionSelector.TryGetValue(FuncIndex, out string FunctionData)
            && FunctionData.Contains("NativeCall<"))
            {
                string FunctionVariables = Regex.Split(FunctionData, "NativeCall<")[1], FriendlyHookName = FuncCombo.Text;
                FunctionVariables = FunctionVariables.Substring(0, FunctionVariables.IndexOf('(') - 1).Replace(" * ", "* ");
                FriendlyHookName = FriendlyHookName.Replace("* >", "*>").Replace(" *>", "*>").Replace("()", "").Replace(")", "");
                if (FriendlyHookName.Contains(" ")) FriendlyHookName = FriendlyHookName.Split(' ')[1];
                if (FriendlyHookName.Contains("(")) FriendlyHookName = FriendlyHookName.Split('(')[0];
                string TrampFunc = "", Tramp = "DECLARE_HOOK(" + StructCombo.Text + "_" + FriendlyHookName;
                // DECLARE_HOOK ARGS
                if (FunctionVariables.Contains(", "))
                {
                    bool AddedClass = false;
                    string[] Vars = Regex.Split(FunctionVariables, ", ");
                    foreach (string s in Vars)
                    {
                        Tramp += ", " + s;
                        if (!AddedClass) //Add Structure to args
                        {
                            Tramp += ", " + StructCombo.Text + "*";
                            AddedClass = true;
                        }
                    }
                    TrampFunc = Vars[0] + " ";
                }
                else
                {                                            //Add Structure to args
                    Tramp += ", " + FunctionVariables + ", " + StructCombo.Text + "*";
                    TrampFunc = FunctionVariables + " ";
                }
                Tramp += ");";
                TrampFunc += " Hook_" + StructCombo.Text + "_" + FriendlyHookName + "(" + StructCombo.Text + "* _this";
                string Variables = FuncCombo.Text;
                int FindIndex;
                if ((FindIndex = Variables.IndexOf('(')) != -1)
                {
                    Variables = Variables.Remove(0, FindIndex + 1);
                    FindIndex = Variables.IndexOf(')');
                    Variables = Variables.Substring(0, FindIndex + 1).Replace(" **", "**").Replace(" *", "*").Replace("enum ", "enum'");
                    TrampFunc += (Variables.Length > 1 ? ", " + Variables.Replace("enum'", "enum ") : ")") + "\n{\n" + (TrampFunc.StartsWith("void") ? "    " : "   return ");
                    TrampFunc += StructCombo.Text + "_" + FriendlyHookName + "_original(_this";
                    if (Variables.Length > 1)
                    {
                        string[] Vars = Regex.Split(Variables, ", ");
                        foreach (string s in Vars)
                            if (s.Contains(" ")) TrampFunc += ", " + s.Split(' ')[1].Replace(")", "");
                        TrampFunc += ");\n}";
                    }
                    else TrampFunc += ");\n}";
                    richTextBox1.AppendText(Tramp + Environment.NewLine + Environment.NewLine + TrampFunc + Environment.NewLine + Environment.NewLine
                        + "ArkApi::GetHooks().SetHook(\"" + StructCombo.Text + "." + FriendlyHookName + "\", &Hook_" + StructCombo.Text + "_" + FriendlyHookName + ", &" + StructCombo.Text + "_" + FriendlyHookName + "_original);" + Environment.NewLine + Environment.NewLine
                        + "ArkApi::GetHooks().DisableHook(\"" + StructCombo.Text + "." + FriendlyHookName + "\", &Hook_" + StructCombo.Text + "_" + FriendlyHookName + ");");
                    Clipboard.SetText(richTextBox1.Text);
                }
            }
        }

        private void FuncCombo_TextUpdate(object sender, EventArgs e)
        {
            FuncCombo.Items.Clear();
            FunctionIndexer.Clear();
            if (FunctionInfo.TryGetValue(ClassCombo.SelectedIndex, out StructureSelector) && StructureSelector.TryGetValue(StructCombo.Text, out FunctionSelector))
            {
                string FuncName;
                foreach (KeyValuePair<int, string> func in FunctionSelector)
                {
                    if (func.Value.Contains(" { "))
                    {
                        FuncName = Regex.Split(func.Value, " { ")[0].Replace(" * ", "* ");
                        if (FuncName.ToLower().Contains(FuncCombo.Text.ToLower()))
                        {
                            FunctionIndexer.Add(FuncCombo.Items.Count, func.Key);
                            FuncCombo.Items.Add(FuncName);
                        }
                    }
                    else
                    {
                        FuncName = func.Value.Replace(" * ", "* ");
                        if (FuncName.ToLower().Contains(FuncCombo.Text.ToLower()))
                        {
                            FunctionIndexer.Add(FuncCombo.Items.Count, func.Key);
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