.. _setup:

Setting-up the system
---------------------

The first thing needed to profit from the different tools offered in this
package is to start **defining the set of files** to work with, this means,
a :class:`hep_rfm.Table` object.
A :class:`hep_rfm.Table` is simply a dictionary with strings as keys,
corresponding to the names of the files, and as values, :class:`FileInfo`
objects.
The latter stores the information needed by this package to work.
This information corresponds to:

- **Name**: this is the user identifier of the file. \
  In a table there can only be one file with the same name.
- **Path**: the path to the file. This can be either a local or remote path. \
  In the last case, the possibility to work with both \
  **ssh and xrootd protocols** is provided.
- **Time-stamp**: the system time-stamp of the file, stored as a float value. \
  This will be needed to determine which files are newer.
- **File ID**: this is the result of hashing the file. For this purpose, the \
  function :func:`hep_rfm.rfm_hash` is used. \
  The result allows to evaluate the integrity of the file, and to determine if \
  two files aiming to be the same are different. Afterwards, the time-stamp \
  will be used to determine which one is newer.

Generally, it will become difficult for the user to generate a table by hand
even using the functions here provided.
A much easier method, makes use of the script **hep-rfm-table**, supplied
together with this package.
**This script allows to create, modify and update tables directly from the
terminal**, avoiding the creation of custom python scripts for that purpose.
To start-up, create a empty directory and then type:

.. code-block:: console

   $ hep-rfm-table -h

You can now see the different options we have to work with tables.
We will start by creating a new table.

.. code-block:: console

   $ hep-rfm-table table.db create
   $ ls
   table.db

We can see the contents on the table at any point by typing

.. code-block:: console

   $ hep-rfm-table table.db display
   No entries found in "table.db"

which is telling us that there are no entries in the table.
Let's add some then.
We will start by creating a new file, with some content, and then we will add it to the table.

.. code-block:: console

   $ echo "My first table file" >> file1.txt
   $ hep-rfm-table table.db add file1 file1.txt
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                    	tmstp             	fid                                                                                                                             
   file1	/home/user/rfm/file1.txt	1532621260.6913095	202444300d48c9387406493590cc693a81c011f1c112d7c8916b27ff9e5daad32a679e4471974b2169c4d22738a17a92405d8948bf5b1d2d8b69c235a65f991b

So we have added our first file.
In order to do this we had to give the name of the file and the path to it.
You can see that we have added a file named *file1*, that the path to the file has been automatically expanded, to match a global path, and that the time-stamp and file ID have been extracted.
Now let's create two more files and add them to the table as well.

.. code-block:: console

   $ echo "This is the second file" >> file2.txt
   $ echo "This is the third file" >> file3.txt
   $ hep-rfm-table table.db add_massive file2.txt file3.txt
   Contents of table "table.db"
   name 	path                    	tmstp             	fid                                                                                                                             
   file1	/home/user/rfm/file1.txt	1532621260.6913095	202444300d48c9387406493590cc693a81c011f1c112d7c8916b27ff9e5daad32a679e4471974b2169c4d22738a17a92405d8948bf5b1d2d8b69c235a65f991b
   file2	/home/user/rfm/file2.txt	1532621261.0113027	cad9a71a7902d13f44db95a00ccc75a52ce12dd080a65d53e09736d652aa8048bc1e02e4a85508544f8578a0c6b245b9f6df23e6b909b4445bdfc2d4d41c4e63
   file3	/home/user/rfm/file3.txt	1532621261.0113027	ce53225da9b363e3db71b188605a88c90ac9e6eb46a512775f2612ff09956f33c97fd2851b94d5812bb7b97e2e7ea7c9c2a66bd148f2c570ecbfdbf346189ff9

With this command one can add easily new files, given only their paths, where
the names will be extracted from the name of the files themselves, without
extensions.
Frequently one will have a whole system of files stored in different directories
and subdirectories.
This can easlity handed running another mode, which will take all the files
within a directory and add them to the table.
We can also specify a regular expression, so only the files whose names
(including the extension) match that given.

.. code-block:: console

   $ mkdir -p subdir/subsubdir
   $ echo "This is the fourth file" >> subdir/file4.txt
   $ echo "This is the fifth file" >> subdir/subsubdir/file5.txt
   $ echo "This is the sixth file" >> subdir/subsubdir/file6.dt
   $ hep-rfm-table table.db add_from_dir . --regex .*.txt
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                                     	tmstp             	fid                                                                                                                             
   file1	/home/user/rfm/file1.txt                 	1532621260.6913095	202444300d48c9387406493590cc693a81c011f1c112d7c8916b27ff9e5daad32a679e4471974b2169c4d22738a17a92405d8948bf5b1d2d8b69c235a65f991b
   file2	/home/user/rfm/file2.txt                 	1532621261.0113027	cad9a71a7902d13f44db95a00ccc75a52ce12dd080a65d53e09736d652aa8048bc1e02e4a85508544f8578a0c6b245b9f6df23e6b909b4445bdfc2d4d41c4e63
   file3	/home/user/rfm/file3.txt                 	1532621261.0113027	ce53225da9b363e3db71b188605a88c90ac9e6eb46a512775f2612ff09956f33c97fd2851b94d5812bb7b97e2e7ea7c9c2a66bd148f2c570ecbfdbf346189ff9
   file4	/home/user/rfm/subdir/file4.txt          	1532621261.3512957	8a9dcf90d2a0bcb95d8fdbaf9c86e2beb0b4fe0825e823742f6f5808621e89f0b98a8266c6135e2e65fbb108ddac4448af668a88d5b736445f8e71065c0b434f
   file5	/home/user/rfm/subdir/subsubdir/file5.txt	1532621261.3512957	86f2f9bb445df1e6281869a36b5c0f1d4cddc5edc722b2c458ec09053713a3836a8376d1421cc9c9329b60a4223812839e76d75702dc27d7c03bf0d00b054906

You can see that we have included all files in the current directory, but from
*file6.dt*, which did not match the given regular expression.
If we remove the regular expression requirement, then it is included

.. code-block:: console

   $ hep-rfm-table table.db add_from_dir .
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                                     	tmstp             	fid                                                                                                                             
   file1	/home/user/rfm/file1.txt                 	1532621260.6913095	202444300d48c9387406493590cc693a81c011f1c112d7c8916b27ff9e5daad32a679e4471974b2169c4d22738a17a92405d8948bf5b1d2d8b69c235a65f991b
   file2	/home/user/rfm/file2.txt                 	1532621261.0113027	cad9a71a7902d13f44db95a00ccc75a52ce12dd080a65d53e09736d652aa8048bc1e02e4a85508544f8578a0c6b245b9f6df23e6b909b4445bdfc2d4d41c4e63
   file3	/home/user/rfm/file3.txt                 	1532621261.0113027	ce53225da9b363e3db71b188605a88c90ac9e6eb46a512775f2612ff09956f33c97fd2851b94d5812bb7b97e2e7ea7c9c2a66bd148f2c570ecbfdbf346189ff9
   file4	/home/user/rfm/subdir/file4.txt          	1532621261.3512957	8a9dcf90d2a0bcb95d8fdbaf9c86e2beb0b4fe0825e823742f6f5808621e89f0b98a8266c6135e2e65fbb108ddac4448af668a88d5b736445f8e71065c0b434f
   file5	/home/user/rfm/subdir/subsubdir/file5.txt	1532621261.3512957	86f2f9bb445df1e6281869a36b5c0f1d4cddc5edc722b2c458ec09053713a3836a8376d1421cc9c9329b60a4223812839e76d75702dc27d7c03bf0d00b054906
   file6	/home/user/rfm/subdir/subsubdir/file6.dt 	1532621261.3512957	5bd329091112ab1c712523d2064d0ca9e4dc2b52095852ae1d314924c0bf6e1d508e269d5a1f5a92d793ad5d285b7d2c44ba1840a9981e9a3c0a86ef24e7ad28
   table	/home/user/rfm/table.db                  	1532621261.5152922	0cc234324fd87b813ec9fba94aea29aa80176b8a2a7a14247d9f5cd50556b90c73c99f2e2cba928d03e508dbea8fbfa758f70e3e98242a5ffbbed2231687090d

You can see that we have also included the table itself.
This is very dangerous, and must be avoided, since it will lead to a replacement
of the table files when working with :class:`hep_rfm.Manager`.
Usually it is preferred that the files are located in a sub-directory, and the
table file in the parent directory, so there are no conflicts.
To return to a safe status, let's remove the table entry, put everything on a
new directory and add the files again.

.. code-block:: console

   $ hep-rfm-table table.db remove table
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                                     	tmstp             	fid                                                                                                                             
   file1	/home/user/rfm/file1.txt                 	1532621260.6913095	202444300d48c9387406493590cc693a81c011f1c112d7c8916b27ff9e5daad32a679e4471974b2169c4d22738a17a92405d8948bf5b1d2d8b69c235a65f991b
   file2	/home/user/rfm/file2.txt                 	1532621261.0113027	cad9a71a7902d13f44db95a00ccc75a52ce12dd080a65d53e09736d652aa8048bc1e02e4a85508544f8578a0c6b245b9f6df23e6b909b4445bdfc2d4d41c4e63
   file3	/home/user/rfm/file3.txt                 	1532621261.0113027	ce53225da9b363e3db71b188605a88c90ac9e6eb46a512775f2612ff09956f33c97fd2851b94d5812bb7b97e2e7ea7c9c2a66bd148f2c570ecbfdbf346189ff9
   file4	/home/user/rfm/subdir/file4.txt          	1532621261.3512957	8a9dcf90d2a0bcb95d8fdbaf9c86e2beb0b4fe0825e823742f6f5808621e89f0b98a8266c6135e2e65fbb108ddac4448af668a88d5b736445f8e71065c0b434f
   file5	/home/user/rfm/subdir/subsubdir/file5.txt	1532621261.3512957	86f2f9bb445df1e6281869a36b5c0f1d4cddc5edc722b2c458ec09053713a3836a8376d1421cc9c9329b60a4223812839e76d75702dc27d7c03bf0d00b054906
   file6	/home/user/rfm/subdir/subsubdir/file6.dt 	1532621261.3512957	5bd329091112ab1c712523d2064d0ca9e4dc2b52095852ae1d314924c0bf6e1d508e269d5a1f5a92d793ad5d285b7d2c44ba1840a9981e9a3c0a86ef24e7ad28
   $ mkdir files
   $ mv subdir *.txt files/.
   $ hep-rfm-table table.db add_from_dir files
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                                           	tmstp             	fid                                                                                                                             
   file1	/home/user/rfm/files/file1.txt                 	1532621260.6913095	202444300d48c9387406493590cc693a81c011f1c112d7c8916b27ff9e5daad32a679e4471974b2169c4d22738a17a92405d8948bf5b1d2d8b69c235a65f991b
   file2	/home/user/rfm/files/file2.txt                 	1532621261.0113027	cad9a71a7902d13f44db95a00ccc75a52ce12dd080a65d53e09736d652aa8048bc1e02e4a85508544f8578a0c6b245b9f6df23e6b909b4445bdfc2d4d41c4e63
   file3	/home/user/rfm/files/file3.txt                 	1532621261.0113027	ce53225da9b363e3db71b188605a88c90ac9e6eb46a512775f2612ff09956f33c97fd2851b94d5812bb7b97e2e7ea7c9c2a66bd148f2c570ecbfdbf346189ff9
   file4	/home/user/rfm/files/subdir/file4.txt          	1532621261.3512957	8a9dcf90d2a0bcb95d8fdbaf9c86e2beb0b4fe0825e823742f6f5808621e89f0b98a8266c6135e2e65fbb108ddac4448af668a88d5b736445f8e71065c0b434f
   file5	/home/user/rfm/files/subdir/subsubdir/file5.txt	1532621261.3512957	86f2f9bb445df1e6281869a36b5c0f1d4cddc5edc722b2c458ec09053713a3836a8376d1421cc9c9329b60a4223812839e76d75702dc27d7c03bf0d00b054906
   file6	/home/user/rfm/files/subdir/subsubdir/file6.dt 	1532621261.3512957	5bd329091112ab1c712523d2064d0ca9e4dc2b52095852ae1d314924c0bf6e1d508e269d5a1f5a92d793ad5d285b7d2c44ba1840a9981e9a3c0a86ef24e7ad28

So now the entry *table* has been removed.
The idea behind this is not only to have a way to keep our data files organized,
but also to be able to **keep files synchronized** in different hosts.
This means that we will have a *main* place, where we would be preferably
placing our new versions of the files, and from there we would be updating the
other locations.
In order to do any modification on a remote we need to authenticate, and
we would need to do it for every single file we want to update.
Using **ssh keys** is thus the preferred way to handle this inconvenient,
or make sure that the target host is directly accessible from your current one.
In the **remote host**, we will need to have **another table** with the paths
in it.
However, we should **specify the path but not the time-stamp or file ID**, since
we will not have a file there.
This is solved adding the *-\\-bare* option to *add* and *add_massive*.
The mode *add_from_dir* will not make sense to be used here, since we do not
have files there.
This means that we could type

.. code-block:: console

   $ ssh username@host.com
   $ mkdir files
   $ hep-rfm-table table.db create
   $ hep-rfm-table table.db add files/file1.txt --bare --remote @host.com
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                                	tmstp             	fid 
   file1	@host.com:/home/user/files/file1.txt	0.0               	none

So you can see that the time-stamp and file ID are filled with default values,
which are chosen so they do not cause conflicts.
Note that we have **specified the remote path** *@host.com*.
This will be needed afterwards to correctly update the files.
The other files will be added in a row, by typing

.. code-block:: console

   $ hep-rfm-table table.db add_massive `a=""; for i in {2..7};do echo files/file$i.txt; done; echo $a` --bare --remote @host.com
   $ hep-rfm-table table.db display
   Contents of table "table.db"
   name 	path                                                 	tmstp	fid 
   file1	@host.com:/home/user/files/file1.txt                 	0.0  	none
   file2	@host.com:/home/user/files/file2.txt                 	0.0  	none
   file3	@host.com:/home/user/files/file3.txt                 	0.0  	none
   file4	@host.com:/home/user/files/subdir/file4.txt          	0.0  	none
   file5	@host.com:/home/user/files/subdir/subsubdir/file5.txt	0.0  	none
   file6	@host.com:/home/user/files/subdir/subsubdir/file6.txt	0.0  	none

Note that here we have included all the files in the same directory.
Files do not need to have similar paths in different hosts.
One we have done this, we are ready to see how we can access the files
and kee data updated in both sites, in the next section: :ref:`file-management`.