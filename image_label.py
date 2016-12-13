#!/Library/Frameworks/Python.framework/Versions/2.7/bin/python


import subprocess

in_dir_name = 'outputs/bulbpower/'
out_dir_name = 'outputs/bulbpower-labeled/'

def go(file_name):
    num = file_name[3:-4]
    text = 'Power: ' + num
    output_file_name = 'out_labeled' + num + '.png'
    cmd = [
        'convert',
        in_dir_name + file_name,
        '-fill', '#0008',
        '-draw', 'rectangle 5, 128, 114, 145',
        '-fill', 'white',
        '-pointsize', '30', 
        '-annotate', '+15+40',
        text,
        out_dir_name + output_file_name,
     ]
    # print 'running', cmd
    # print ' '.join(cmd)
    subprocess.check_call(cmd)

# convert a.png \
#         -fill '#0008' -draw 'rectangle 5,128,114,145' \
#         -fill white  -pointsize 20 -annotate +10+20 'TEXT' \
#         a_label.png

if __name__ == '__main__':
    file_names = subprocess.check_output(['ls', in_dir_name]).split()
    print file_names
    for file_name in file_names:
        go(file_name)
