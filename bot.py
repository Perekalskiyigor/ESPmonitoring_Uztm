from telegram import InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import Updater, CommandHandler, CallbackQueryHandler
import pyodbc
from tabulate import tabulate

# Замените на ваш токен бота
TOKEN = "6666778333:AAGL2I3DzPrs-MJ8k05hIqaRY4lduPBdHUU"

# Замените на ваши данные для подключения к MS SQL Server
server = 'extdb.uralmash.ru'
database = 'BI_TEST'
username = 'Perekalskiy_igor'
password = 'Fnkfynblf198714'
driver = '{ODBC Driver 17 for SQL Server}'

def start(update, context):
    update.message.reply_text('Привет! Нажмите на кнопку для получения данных из базы данных:')
    keyboard = [
        [InlineKeyboardButton("Получить данные", callback_data='get_data')]
    ]
    reply_markup = InlineKeyboardMarkup(keyboard)
    update.message.reply_text('Выберите действие:', reply_markup=reply_markup)

def button(update, context):
    query = update.callback_query
    query.answer()

    if query.data == 'get_data':
        try:
            cnxn = pyodbc.connect('DRIVER='+driver+';SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
            cursor = cnxn.cursor()
            cursor.execute("SELECT TOP 1 [Data],[IP], Sensor1, Sensor2, Sensor3, Sensor4, Sensor5, Sensor6  FROM [BI_TEST].[dbo].[ESPTable]  ORDER BY [Data] DESC")
            row = cursor.fetchone()
            while row:
            # Format the message
                message = f"Дата показаний: {row[0]}\n Sensor1 = {row[2]}\n Sensor2 = {row[3]}\n Sensor3 = {row[3]}\n Sensor4 = {row[4]}\n Sensor5 = {row[5]}\n Sensor6 = {row[6]}\n"
                query.edit_message_text(text=message)
                row = cursor.fetchone()
            cnxn.close()
            print("Successfully connected to the database and executed the query.")
        except Exception as e:
            print(f"An error occurred: {e}")
            query.edit_message_text(text="An error occurred while connecting to the database or executing the query.")

def main():
    updater = Updater(TOKEN, use_context=True)
    dp = updater.dispatcher
    dp.add_handler(CommandHandler('start', start))
    dp.add_handler(CallbackQueryHandler(button))
    updater.start_polling()
    updater.idle()

if __name__ == '__main__':
    main()


'''    if query.data == 'get_data':
        try:
            cnxn = pyodbc.connect('DRIVER='+driver+';SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
            cursor = cnxn.cursor()
            cursor.execute("SELECT TOP 1 [Data],[IP], Sensor1, Sensor2, Sensor3, Sensor4, Sensor5 FROM [BI_TEST].[dbo].[ESPTable]")
            row = cursor.fetchone()
            while row:
                
                
                # Create a table
                table = [['Date', 'Sensor1', 'Sensor2', 'Sensor3', 'Sensor4',  'Sensor5'], [row[0], row[1], row[2], row[3], row[4], row[5]]]
                # Create a figure and a table
                fig, ax = plt.subplots()
                ax.axis('tight')
                ax.axis('off')
                
                the_table = ax.table(cellText=table, colLabels=None, cellLoc = 'center', loc='center')

                # Change the font size and weight
                for cell in the_table._cells.values():
                    cell.get_text().set_fontsize(20)
                    cell.get_text().set_fontweight('bold')
                
                # Save the table as an image
                fig.savefig('table.png')
                cnxn.close()
                print("Successfully connected to the database and executed the query.")
                # Open the image by specifying the image path.
                image_path = "table.png"
                image_file = Image.open(image_path)

                # Send the image
                with open('table.png', 'rb') as img:
                    context.bot.send_photo(chat_id=query.message.chat_id, photo=img)  # Use context.bot here
                row = cursor.fetchone()
        except Exception as e:
            print(f"An error occurred: {e}")
            query.edit_message_text(text="An error occurred while connecting to the database or executing the query.")        '''