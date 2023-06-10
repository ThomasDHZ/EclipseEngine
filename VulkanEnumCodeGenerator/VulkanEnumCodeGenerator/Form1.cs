using System.Linq;
using System.Linq.Expressions;

namespace VulkanEnumCodeGenerator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void ConvertButton_Click(object sender, EventArgs e)
        {
            var enumName = VulkanEnumTextBox.Text;
            var vulkanEnumRichTextBox = VulkanEnumRichTextBox.Text;

            var enumNameList = new List<string>();
            for (int x = 0; x < VulkanEnumRichTextBox.Lines.Count(); x++)
            {
                var index = VulkanEnumRichTextBox.Lines[x].ToString().IndexOf('=');
                var split = VulkanEnumRichTextBox.Lines[x].ToString().Split('=').First();
                split = split.Replace(" ", "");
                enumNameList.Add(split);
            }

            ReturnCodeRichTextBox.Text = $"const char* {enumName}EnumList[{enumNameList.Count()}] ";
            ReturnCodeRichTextBox.Text += "{ \n";
            for (int x = 0; x < enumNameList.Count(); x++)
            {
                ReturnCodeRichTextBox.Text += $"\"";
                ReturnCodeRichTextBox.Text += enumNameList[x];
                ReturnCodeRichTextBox.Text += $"\"";
                ReturnCodeRichTextBox.Text += $", \n";
            }
            ReturnCodeRichTextBox.Text += "}; \n\n";

            ReturnCodeRichTextBox.Text += $"struct {enumName}Converter \n";
            ReturnCodeRichTextBox.Text += "{ \n";
            ReturnCodeRichTextBox.Text += $"static const char* SelectionToString({enumName} input) \n";
            ReturnCodeRichTextBox.Text += "{ \n";
            ReturnCodeRichTextBox.Text += "switch (input) \n";
            ReturnCodeRichTextBox.Text += "{ \n";
            for (int x = 0; x < enumNameList.Count(); x++)
            {
                ReturnCodeRichTextBox.Text += $"case {enumNameList[x]}: return \"{ enumNameList[x]}\"; break; \n";
            }
            ReturnCodeRichTextBox.Text += "} \n";
            ReturnCodeRichTextBox.Text += "} \n\n";


            ReturnCodeRichTextBox.Text += $"static {enumName} SelectionToEnum(std::string input)\n";
            ReturnCodeRichTextBox.Text += "{\n";

            ReturnCodeRichTextBox.Text += "{\n";
            for (int x = 0; x < enumNameList.Count(); x++)
            {
                ReturnCodeRichTextBox.Text += $"if (input == \"{enumNameList[x]}\") return {enumNameList[x]};\n";
            }
            ReturnCodeRichTextBox.Text += "}\n";
            ReturnCodeRichTextBox.Text += "};\n";
            ReturnCodeRichTextBox.Text += "};\n";
        }
    }
}