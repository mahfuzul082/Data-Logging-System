import pandas as pd
import numpy as np

## files to be processed

x = '.csv' #change file name
y = '.csv' #change file name
z = '.csv' #change file name

z0 = 'power_windspeed.csv'

##for time-window bare wt

data = pd.read_csv(x)
data1 = pd.read_csv(x, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
data['Avg 3min'] = data['Power (W)'].rolling(36).mean()
data['Avg 5min'] = data['Power (W)'].rolling(60).mean()
data['Avg 10min'] = data['Power (W)'].rolling(120).mean()
data.to_csv(x, index = False)
df = data1.resample('10min', label = 'right')['Power (W)'].mean()
df.rename('Avg10min', inplace=True)
#print(df)
#df = pd.DataFrame(df)
df.to_csv("10avg.csv", index = True)
df1 = pd.read_csv("10avg.csv", skiprows = [1]) ##skipfooter,engine = 'python'
df1['max'] = df1['Avg10min'].max()
#df1.rename('max', inplace=True)
#df1.to_csv("10max.csv", index = True)
df1['avg'] = df1['Avg10min'].mean()
#df2.rename('Avg', inplace=True)
df1.to_csv("10avg.csv", index = False)
dff = data1.resample('1H')['Power (W)'].max()
dff.rename('max', inplace=True)
dff.to_csv("max.csv", index = True)
dff1 = data1.resample('1min')['Power (W)'].mean()
dff1 = dff1.resample('1H').sum()
dff1 = dff1/60000
dff1.rename('Energy kWh', inplace=True)
dff1.to_csv("energy.csv", index = True)
csv1 = pd.read_csv(x)
csv2 = pd.read_csv('10avg.csv')
csv3 = pd.read_csv('max.csv')
csv4 = pd.read_csv('energy.csv')
concate = pd.concat((csv1, csv2, csv3, csv4), axis = 'columns')
concate.to_csv(x, index = False)

##for time-window fas wt

data2 = pd.read_csv(y)
data3 = pd.read_csv(y, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
data2['Avg 3min'] = data2['Power (W)'].rolling(36).mean()
data2['Avg 5min'] = data2['Power (W)'].rolling(60).mean()
data2['Avg 10min'] = data2['Power (W)'].rolling(120).mean()
data2.to_csv(y, index = False)
df2 = data3.resample('10min', label = 'right')['Power (W)'].mean()
df2.rename('Avg10min', inplace=True)
#print(df)
#df = pd.DataFrame(df)
df2.to_csv("10avg.csv", index = True)
df3 = pd.read_csv("10avg.csv", skiprows = [1]) ##skipfooter,engine = 'python'
df3['max'] = df3['Avg10min'].max()
#df1.rename('max', inplace=True)
#df1.to_csv("10max.csv", index = True)
df3['avg'] = df3['Avg10min'].mean()
#df2.rename('Avg', inplace=True)
df3.to_csv("10avg.csv", index = False)
dff2 = data3.resample('1H')['Power (W)'].max()
dff2.rename('max', inplace=True)
dff2.to_csv("max.csv", index = True)
dff3 = data3.resample('1min')['Power (W)'].mean()
dff3 = dff3.resample('1H').sum()
dff3 = dff3/60000
dff3.rename('Energy kWh', inplace=True)
dff3.to_csv("energy.csv", index = True)
csv1 = pd.read_csv(y)
csv2 = pd.read_csv('10avg.csv')
csv3 = pd.read_csv('max.csv')
csv4 = pd.read_csv('energy.csv')
concate = pd.concat((csv1, csv2, csv3, csv4), axis = 'columns')
concate.to_csv(y, index = False)

##for time-window wind speed measurement

data4 = pd.read_csv(z, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
df4 = data4.resample('10min', label = 'right')['Wind speed (m/s)'].mean()
df4.rename('Avg wind speed', inplace=True)
df4.to_csv("10avg.csv", index = True)
csv1 = pd.read_csv(z0)
csv2 = pd.read_csv('10avg.csv', usecols = ['DateTime', 'Avg wind speed'])#, skiprows = [1])
csv3 = pd.read_csv(x, usecols = ['Avg10min'], skipfooter = 2516, engine = 'python')#, skiprows = [1]) #change skipfooter
#print(csv3)
csv3.rename(columns = {'Avg10min':'Bare'}, inplace = True)
csv4 = pd.read_csv(y, usecols = ['Avg10min'], skipfooter = 2520, engine = 'python')#, skiprows = [1]) #change skipfooter
#print(csv4)
csv4.rename(columns = {'Avg10min':'FAS'}, inplace = True)
concate1 = pd.concat((csv2, csv3, csv4), axis = 1)
concate1.to_csv('dummy.csv', index = False)
csv5 = pd.read_csv('dummy.csv')
concate = pd.concat((csv1, csv5), axis = 0)
concate.to_csv(z0, index = False)
data5 = pd.read_csv(z, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
dff4 = data5.resample('10min', label = 'right')['Wind speed (m/s)'].mean()
dff4.rename('Avg10min', inplace=True)
dff4.to_csv(z, index = True)
df5 = pd.read_csv(z) ##skipfooter,engine = 'python'
df5['max'] = df5['Avg10min'].max()
df5['avg'] = df5['Avg10min'].mean()
df5.to_csv(z, index = False)