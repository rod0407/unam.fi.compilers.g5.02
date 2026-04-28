import subprocess
import json
import customtkinter as ctk


def run_compiler(source_code):
    try:
        result = subprocess.run(
            ["./parser_c.exe", source_code], 
            capture_output=True, 
            text=True
        )
        stdout_output = result.stdout.strip()

        if "Parsing error..." in stdout_output or result.returncode != 0:
            return "Parsing error...\n"

        try:
            ast = json.loads(stdout_output)
        except json.JSONDecodeError:
            return "Parsing error... (Salida corrupta del parser en C)\n"
        
        sdt_verified = True
        
        if ast["node_type"] == "VarDecl":
            declared_type = ast["var_type"]
            expression_type = ast["expr_type"]
            
            if declared_type != expression_type:
                if not (declared_type == 'float' and expression_type == 'int'):
                    sdt_verified = False

        output_msg = "Parsing Success!\n"
        if sdt_verified:
            output_msg += "SDT Verified!\n"
        else:
            output_msg += "SDT error...\n"
            
        output_msg += "\n--- Parse Tree (AST) ---\n"
        output_msg += json.dumps(ast, indent=4)
            
        return output_msg

    except FileNotFoundError:
        return "Error: No se encontró parser_c.exe. ¡Asegúrate de compilar el código en C primero!\n"
    except Exception as e:
        return f"Error inesperado: {str(e)}\n"


ctk.set_appearance_mode("Dark") 
ctk.set_default_color_theme("blue") 

class CompilerApp(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Parser & SDT - Equipo 2")
        self.geometry("700x600") 
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        self.title_label = ctk.CTkLabel(self, text="Analizador Sintáctico y Semántico", font=("Roboto", 24, "bold"))
        self.title_label.grid(row=0, column=0, padx=20, pady=(20, 10))

        self.input_textbox = ctk.CTkTextbox(self, font=("Consolas", 16), corner_radius=10)
        self.input_textbox.grid(row=1, column=0, padx=20, pady=10, sticky="nsew")
        
        self.input_textbox.insert("0.0", "int x = 10;")

        self.compile_button = ctk.CTkButton(self, text="▶ Compilar Código", font=("Roboto", 16, "bold"), 
                                            height=40, command=self.on_compile_click)
        self.compile_button.grid(row=2, column=0, padx=20, pady=10)

        self.output_textbox = ctk.CTkTextbox(self, font=("Consolas", 16), height=250, corner_radius=10, 
                                             fg_color="#1E1E1E", text_color="#00FF00")
        self.output_textbox.grid(row=3, column=0, padx=20, pady=(10, 20), sticky="ew")
        self.output_textbox.configure(state="disabled")

    def on_compile_click(self):
        source_code = self.input_textbox.get("0.0", "end").strip()
        
        if not source_code:
            self.update_output("Por favor, ingresa código para compilar.")
            return

        result = run_compiler(source_code)
        self.update_output(result)

    def update_output(self, text):
        self.output_textbox.configure(state="normal")
        self.output_textbox.delete("0.0", "end")
        self.output_textbox.insert("0.0", text)
        self.output_textbox.configure(state="disabled")


if __name__ == "__main__":
    subprocess.run(["gcc", "parser.c", "-o", "parser_c.exe"], capture_output=True)
    
    app = CompilerApp()
    app.mainloop()