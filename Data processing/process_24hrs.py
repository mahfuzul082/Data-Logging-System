import pandas as pd
import numpy as np

##files to be processed

x1 = '.csv' #change file name
y1 = '.csv' #change file name
z1 = '.csv' #change file name

##for 24hrs wind speed measurement

data5 = pd.read_csv(z1, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
dff4 = data5.resample('10min', label = 'right')['Wind speed (m/s)'].mean()
dff5 = data5.resample('10min', label = 'right')['Wind speed (m/s)'].std(ddof=0)
dff4.rename('Avg10min wind', inplace=True)
dff4[''] = dff4.mean()
dff5.rename('RMS fluctuation', inplace=True)
dff6 = dff5/dff4*100
dff6.rename('Turbulence intensity', inplace=True)
dff6[''] = dff6.mean()
dff4.to_csv('dummy1.csv', index = True)
dff5.to_csv('dummy2.csv', index = False)
dff6.to_csv('dummy3.csv', index = False)

csv1 = pd.read_csv('dummy1.csv')
csv2 = pd.read_csv('dummy2.csv')
csv3 = pd.read_csv('dummy3.csv')

concate = pd.concat((csv1,csv2,csv3), axis = 1)
concate.to_csv(z1, index = False)

##for 24hrs bare wt

data6 = pd.read_csv(x1, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
dff7 = data6.resample('10min', label = 'right')['Power (W)'].mean()
dff7.rename('Avg10min power', inplace=True)
dff7.to_csv("10avg.csv", index = True)
dff8 = pd.read_csv("10avg.csv")
dff8['Avg of 10min avg'] = dff8['Avg10min power'].mean()
dff8['Energy (Wh)'] = dff8['Avg10min power']*10/60
dff8['Total Energy (Wh)'] = dff8['Energy (Wh)'].sum()
dff8.to_csv("10avg.csv", index = False)

csv1 = pd.read_csv(x1)
csv2 = pd.read_csv('10avg.csv')
csv3 = pd.read_csv(z1, usecols=lambda x: x != "DateTime")

concate = pd.concat((csv1,csv2,csv3), axis = 1)
concate.to_csv(x1, index = False)

##for 24hrs fas wt

data8 = pd.read_csv(y1, parse_dates = {'DateTime': ['Date', 'Time']}, index_col = 'DateTime')   
dff9 = data8.resample('10min', label = 'right')['Power (W)'].mean()
dff9.rename('Avg10min power', inplace=True)
dff9.to_csv("10avg.csv", index = True)
dff10 = pd.read_csv("10avg.csv")
dff10['Avg of 10min avg'] = dff10['Avg10min power'].mean()
dff10['Energy (Wh)'] = dff10['Avg10min power']*10/60
dff10['Total Energy (Wh)'] = dff10['Energy (Wh)'].sum()
dff10.to_csv("10avg.csv", index = False)

csv1 = pd.read_csv(y1)
csv2 = pd.read_csv('10avg.csv')
csv3 = pd.read_csv(z1, usecols=lambda x: x != "DateTime")

concate = pd.concat((csv1,csv2,csv3), axis = 1)
concate.to_csv(y1, index = False)

## for 24hrs % increase

data9 = pd.read_csv(x1, usecols = ['DateTime','Avg10min power'])
data10 = pd.read_csv(y1, usecols = ['DateTime','Avg10min power'])
dff11 = data10
dff11['Avg10min power'] = (data10['Avg10min power']-data9['Avg10min power'])/data9['Avg10min power']*100
dff11['Avg10min power'].replace([np.inf], np.NaN, inplace=True)
dff11['Avg %power increase'] = dff11['Avg10min power'].mean()
dff11.rename(columns={'Avg10min power':'% increase avg10min power'}, inplace=True)
dff11.to_csv("%10min_increase.csv", index = False)
