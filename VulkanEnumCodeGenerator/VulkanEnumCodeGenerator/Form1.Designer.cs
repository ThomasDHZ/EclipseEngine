namespace VulkanEnumCodeGenerator
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.VulkanEnumRichTextBox = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.ReturnCodeRichTextBox = new System.Windows.Forms.RichTextBox();
            this.ConvertButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.VulkanEnumTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // VulkanEnumRichTextBox
            // 
            this.VulkanEnumRichTextBox.Location = new System.Drawing.Point(29, 73);
            this.VulkanEnumRichTextBox.Name = "VulkanEnumRichTextBox";
            this.VulkanEnumRichTextBox.Size = new System.Drawing.Size(318, 365);
            this.VulkanEnumRichTextBox.TabIndex = 1;
            this.VulkanEnumRichTextBox.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 55);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(98, 15);
            this.label1.TabIndex = 2;
            this.label1.Text = "Vulkan Enum List";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(470, 55);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(108, 15);
            this.label2.TabIndex = 4;
            this.label2.Text = "Vulkan Enum Code";
            // 
            // ReturnCodeRichTextBox
            // 
            this.ReturnCodeRichTextBox.Location = new System.Drawing.Point(470, 73);
            this.ReturnCodeRichTextBox.Name = "ReturnCodeRichTextBox";
            this.ReturnCodeRichTextBox.Size = new System.Drawing.Size(318, 365);
            this.ReturnCodeRichTextBox.TabIndex = 3;
            this.ReturnCodeRichTextBox.Text = "";
            // 
            // ConvertButton
            // 
            this.ConvertButton.Location = new System.Drawing.Point(371, 24);
            this.ConvertButton.Name = "ConvertButton";
            this.ConvertButton.Size = new System.Drawing.Size(75, 23);
            this.ConvertButton.TabIndex = 5;
            this.ConvertButton.Text = "Convert";
            this.ConvertButton.UseVisualStyleBackColor = true;
            this.ConvertButton.Click += new System.EventHandler(this.ConvertButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(29, 27);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(112, 15);
            this.label3.TabIndex = 6;
            this.label3.Text = "Vulkan Enum Name";
            // 
            // VulkanEnumTextBox
            // 
            this.VulkanEnumTextBox.Location = new System.Drawing.Point(157, 24);
            this.VulkanEnumTextBox.Name = "VulkanEnumTextBox";
            this.VulkanEnumTextBox.Size = new System.Drawing.Size(190, 23);
            this.VulkanEnumTextBox.TabIndex = 7;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.VulkanEnumTextBox);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.ConvertButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.ReturnCodeRichTextBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.VulkanEnumRichTextBox);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private RichTextBox VulkanEnumRichTextBox;
        private Label label1;
        private Label label2;
        private RichTextBox ReturnCodeRichTextBox;
        private Button ConvertButton;
        private Label label3;
        private TextBox VulkanEnumTextBox;
    }
}