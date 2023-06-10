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


        private void ReturnCodeRichTextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void VulkanEnumRichTextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void VulkanEnumTextBox_TextChanged(object sender, EventArgs e)
        {

        }

        private void ConvertButton_Click(object sender, EventArgs e)
        {
            var enumName = VulkanEnumTextBox.Text;
            var vulkanEnumRichTextBox = VulkanEnumRichTextBox.Text;

            var enumNameList = new List<string>();
            for(int x = 0; x < VulkanEnumRichTextBox.Lines.Count(); x++)
            {
                var index = VulkanEnumRichTextBox.Lines[x].ToString().IndexOf('=');
                enumNameList.Add(VulkanEnumRichTextBox.Lines[x].ToString().Split('=').First());
            }
            
            ReturnCodeRichTextBox.Text = $"const char* {enumName}EnumList[{enumNameList.Count()}] ";
            ReturnCodeRichTextBox.Text += "{";
            for (int x = 0; x < enumNameList.Count(); x++)
            {
                ReturnCodeRichTextBox.Text += $"\"{enumNameList[x]}, {Environment.NewLine}\"";
            }
            ReturnCodeRichTextBox.Text += "};" + Environment.NewLine;

            ReturnCodeRichTextBox.Text += $"struct {enumName}Mode";
            ReturnCodeRichTextBox.Text += "{";
                ReturnCodeRichTextBox.Text += $"static const char* SelectionToString({enumName} input)";
                ReturnCodeRichTextBox.Text += "{";
                    ReturnCodeRichTextBox.Text += "switch (input)";
                    ReturnCodeRichTextBox.Text += "{";
                        for (int x = 0; x < VulkanEnumRichTextBox.Lines.Count(); x++)
                        {
                            ReturnCodeRichTextBox.Text += $"case {VulkanEnumRichTextBox.Lines[x]}: return \"{ VulkanEnumRichTextBox.Lines[x]}\"; break;";
                        }
                    ReturnCodeRichTextBox.Text += "}";
                ReturnCodeRichTextBox.Text += "}";


                ReturnCodeRichTextBox.Text += $"static const char* SelectionToString({enumName} input)";
                ReturnCodeRichTextBox.Text += "{";
                    ReturnCodeRichTextBox.Text += "switch (input)";
                    ReturnCodeRichTextBox.Text += "{";
                        for (int x = 0; x < VulkanEnumRichTextBox.Lines.Count(); x++)
                        {
                            ReturnCodeRichTextBox.Text += $"if (input == \"{VulkanEnumRichTextBox.Lines[x]}\") return {VulkanEnumRichTextBox.Lines[x]};";
                        }
                    ReturnCodeRichTextBox.Text += "}";
                ReturnCodeRichTextBox.Text += "}";
            ReturnCodeRichTextBox.Text += "};" + Environment.NewLine;


        }
    }
}